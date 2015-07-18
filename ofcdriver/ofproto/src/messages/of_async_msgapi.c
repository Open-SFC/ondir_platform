/**
 * Copyright (c) 2012, 2013  Freescale.
 *  
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

/*!\file of_async_msgapi.c
 * Author     : Rajesh Madabushi <rajesh.madabushi@freescale.com>
 * Date       : November, 2012  
 * Description: 
 * This file contains defintions different asynchrnous messaging APIs provided to 
 * North bound interface. 
 */

/* INCLUDE_START ************************************************************/
#include "controller.h"
#include "of_utilities.h"
#include "of_multipart.h"
#include "of_msgapi.h"
#include "cntrl_queue.h"
#include "cntlr_tls.h"
#include "cntlr_transprt.h"
#include "cntlr_event.h"
#include "dprmldef.h"
#include "ttprmgdef.h"
#include "pktmbuf.h"
/* INCLUDE_END **************************************************************/


struct of_app_hook_info*
of_get_async_msg_app_hook(struct async_event_info *event_info_p,
                          uint8_t                  event_type,
                          uint8_t                  priority);


void
of_insert_app_hook_in_a_dscnding_priority(struct async_event_info      *event_info_p,
                                          uint8_t                       event_type,
                                          struct of_app_hook_info      *app_entry);

/* APIs for applications to reigster callbacks to receive events */
int32_t
of_register_asynchronous_message_hook(char      *main_ttp_name_p,
                                      uint8_t   table_id,
                                      uint8_t   event_type,
                                      uint8_t   priority,
                                      void     *call_back,
                                      void     *cbk_arg1,
                                      void     *cbk_arg2)
{
   struct async_event_info   *event_info_p=NULL;
   struct of_app_hook_info   *app_entry = NULL;

   int32_t                        status = OF_SUCCESS;

   if( (event_type < OF_FIRST_ASYNC_EVENT) ||
       (event_type > OF_LAST_ASYNC_EVENT)      )
   {
       OF_LOG_MSG(OF_LOG_MOD, OF_LOG_DEBUG,"%s:%d Invalid event(%d) type passed \r\n",__FUNCTION__,__LINE__,event_type);
       return OF_REG_ASYNC_HOOK_INVALID_EVENT_TYPE;
   }

   if( (priority < OF_ASYNC_MSG_APP_MIN_PRIORITY)  ||
       (priority > OF_ASYNC_MSG_APP_MAX_PRIORITY)     )
   {
       OF_LOG_MSG(OF_LOG_MOD, OF_LOG_DEBUG,"%s:%d Invalid Priority(%d) value passed \r\n",__FUNCTION__,__LINE__,priority);
       return OF_REG_ASYNC_HOOK_INVALID_APP_PRIORITY;
   }

   if(call_back == NULL)
   {
       OF_LOG_MSG(OF_LOG_MOD, OF_LOG_DEBUG,"%s:%d NULL value as callback function  \r\n",__FUNCTION__,__LINE__);
       return OF_REG_ASYNC_HOOK_NULL_CALLBACK;
   }

   CNTLR_RCU_READ_LOCK_TAKE();
   do
   {
      OF_LOG_MSG(OF_LOG_MOD, OF_LOG_DEBUG, "tableid:%d",table_id);
      if(get_ttprm_async_event_info(main_ttp_name_p, table_id ,&event_info_p)!=OF_SUCCESS)
      {
        OF_LOG_MSG(OF_LOG_MOD, OF_LOG_DEBUG, "Get ttprm node failed!.");
        status = OF_FAILURE;
        break; 
      }


      app_entry = of_get_async_msg_app_hook(event_info_p, event_type, priority);
      if(app_entry != NULL)
      {
         OF_LOG_MSG(OF_LOG_MOD, OF_LOG_DEBUG,"%s:%d Tyring to regisgter application with Duplication prioirty \r\n", __FUNCTION__,__LINE__);
         status = OF_REG_ASYNC_HOOK_DUPLICATE_APP_PRIOIRTY;
         app_entry = NULL;
         break;
      }
      app_entry = NULL;


      app_entry = (struct of_app_hook_info*)calloc(1,sizeof(struct of_app_hook_info));
      if(app_entry == NULL)
      {
         OF_LOG_MSG(OF_LOG_MOD, OF_LOG_DEBUG,"%s:%d Tyring to regisgter application with Duplication prioirty \r\n", __FUNCTION__,__LINE__);
         status = CNTLR_MEM_ALLOC_ERROR;
         app_entry = NULL;
         break;
      } 
 
      app_entry->priority  = priority;
      app_entry->call_back = call_back;
      app_entry->cbk_arg1  = cbk_arg1;
      app_entry->cbk_arg2  = cbk_arg2;

      /* Inset application in the priority order to application list
       * Applications are maintained in descening order*/
      of_insert_app_hook_in_a_dscnding_priority(event_info_p,
                                                event_type,
                                                app_entry);
  }
  while(0);
  CNTLR_RCU_READ_LOCK_RELEASE();

  if(status != OF_SUCCESS)
  {
     if(app_entry != NULL)
         free(app_entry);
  }

  return status;
}

int32_t
of_deregister_asynchronous_message_hook(char *main_ttp_name_p,
                                      uint8_t   table_id,
                                      uint8_t   event_type,
                                      uint8_t   priority
                                      )
{
  struct of_app_hook_info  *app_entry = NULL, *prev_app_entry_p=NULL;
  struct async_event_info  *event_info_p=NULL;
  int8_t   offset;
  int32_t  status = OF_FAILURE;

  if( (main_ttp_name_p == NULL ))
  {
    OF_LOG_MSG(OF_LOG_MOD, OF_LOG_DEBUG,"Invalid ttp  passed");
    return OF_FAILURE;
  }
  if( (event_type < OF_FIRST_ASYNC_EVENT) ||
		  (event_type > OF_LAST_ASYNC_EVENT)      )
  {
    OF_LOG_MSG(OF_LOG_MOD, OF_LOG_DEBUG,"Invalid event(%d) type passed",event_type);
    return OF_REG_ASYNC_HOOK_INVALID_EVENT_TYPE;
  }

  if( (priority < OF_ASYNC_MSG_APP_MIN_PRIORITY)  ||
			(priority > OF_ASYNC_MSG_APP_MAX_PRIORITY)     )
  {
    OF_LOG_MSG(OF_LOG_MOD, OF_LOG_DEBUG,"Invalid Priority(%d) value passed",priority);
    return OF_REG_ASYNC_HOOK_INVALID_APP_PRIORITY;
  }


  CNTLR_RCU_READ_LOCK_TAKE();
  do
  {
    if(get_ttprm_async_event_info(main_ttp_name_p, table_id ,&event_info_p)!=OF_SUCCESS)
    {
      OF_LOG_MSG(OF_LOG_MOD, OF_LOG_DEBUG, "Get ttprm node failed!.");
      status = OF_FAILURE;
      break; 
    }

    offset = OF_APP_HOOK_LISTNODE_OFFSET;
    OF_LIST_SCAN(event_info_p[event_type].app_list,app_entry,struct of_app_hook_info*,offset)
    {
      if(app_entry->priority != priority)
      {
        prev_app_entry_p = app_entry;
	continue;
      }
      OF_REMOVE_NODE_FROM_LIST( event_info_p[event_type].app_list,app_entry,prev_app_entry_p,offset );
      free(app_entry);
      status=OF_SUCCESS;
      break;
    }
  }
  while(0);
  CNTLR_RCU_READ_LOCK_RELEASE();
  return status;
}
/* Caller MUST use RCU_READ LOCK and RCU_READ  UNlock*/
void
of_insert_app_hook_in_a_dscnding_priority(struct async_event_info      *event_info_p,
                                          uint8_t                       event_type,
                                          struct of_app_hook_info      *app_entry)
{
    struct of_app_hook_info *curr_app_entry = NULL;
    struct of_app_hook_info *prev_app_entry = NULL;
    uint16_t offset;
    offset = OF_APP_HOOK_LISTNODE_OFFSET;
   
    OF_LIST_SCAN((event_info_p[event_type]).app_list,curr_app_entry,struct of_app_hook_info*,offset)
    {
        if(app_entry->priority > curr_app_entry->priority)
        {
           OF_LIST_INSERT_NODE(event_info_p[event_type].app_list,
                               prev_app_entry,
                               app_entry,
                               curr_app_entry,offset);
           return;
        }
        prev_app_entry = curr_app_entry;
    }

    OF_APPEND_NODE_TO_LIST(event_info_p[event_type].app_list,app_entry,offset);
}

/* Caller MUST use RCU_READ LOCK and RCU_READ  unlock*/
struct of_app_hook_info*
of_get_async_msg_app_hook(struct async_event_info *event_info_p,
                          uint8_t                  event_type,
                          uint8_t                  priority)
{
   struct of_app_hook_info *app_entry;
    uint16_t offset;
    offset = OF_APP_HOOK_LISTNODE_OFFSET;

   OF_LIST_SCAN(event_info_p[event_type].app_list,app_entry,struct of_app_hook_info*,offset)
   {
     if(app_entry->priority == priority)
              return app_entry;
   }
   return NULL;
}

int32_t
handover_pkt_in_msg_to_app(uint64_t datapath_handle,
                          struct dprm_domain_entry *domain,
                          uint8_t                   table_id,
                          uint8_t                   msg_id,
                          struct of_msg            *msg,
                          struct ofl_packet_in     *pkt_in)
{
   struct of_app_hook_info  *app_entry;
   int32_t retval = OF_ASYNC_MSG_PASS_TO_NEXT_APP;
   uint16_t offset;
   struct async_event_info      *event_info_p =NULL;
   offset = OF_APP_HOOK_LISTNODE_OFFSET;

   if (msg_id > OF_MAX_NUMBER_ASYNC_MSGS)
   {
      OF_LOG_MSG(OF_LOG_MOD, OF_LOG_ERROR,"%s:%d Invalid message with id=%d  \r\n",__FUNCTION__,__LINE__,msg_id);
      return OF_FAILURE;
   }
#if 0
   domain_table_id = get_domaintableid_by_dptableid(datapath_handle, table_id);
   if(domain_table_id < 0)
   { 
     OF_LOG_MSG(OF_LOG_MOD, OF_LOG_ERROR,"Not able to find the domain table id for the table id=%d",table_id);
     return OF_FAILURE;
   }
   OF_LOG_MSG(OF_LOG_MOD, OF_LOG_ERROR,"domain_table_id=%d table_id=%d", domain_table_id, table_id);
#endif

   if(get_ttprm_async_event_info(domain->ttp_name, table_id ,&event_info_p)!=OF_SUCCESS)
   {
     OF_LOG_MSG(OF_LOG_MOD, OF_LOG_ERROR, "Get ttprm node failed!.");
     return OF_FAILURE;
   }

   OF_LIST_SCAN(event_info_p[msg_id].app_list,
                app_entry,struct of_app_hook_info*,offset)
   {
     if(app_entry != NULL)
     retval = ((of_packet_in_clbk_fn)(app_entry->call_back))(controller_handle,
                                                             domain->domain_handle,
                                                             datapath_handle,
                                                             msg,
                                                             pkt_in,
                                                             app_entry->cbk_arg1,
                                                             app_entry->cbk_arg2);
     if(retval == OF_ASYNC_MSG_CONSUMED)
       break;
     else if(retval == OF_ASYNC_MSG_PASS_TO_NEXT_APP)
       continue;
     else if(retval == OF_ASYNC_MSG_DROP)
     {
       of_get_pkt_mbuf_by_of_msg_and_free(msg);
       break;
     }
     else
     {
        OF_LOG_MSG(OF_LOG_MOD, OF_LOG_ERROR,
           "PKT-IN issue with return value in table = %d , retval = %d", table_id, retval);
     }
   }

   if(retval == OF_ASYNC_MSG_PASS_TO_NEXT_APP) 
   {
       OF_LOG_MSG(OF_LOG_MOD, OF_LOG_DEBUG, "Freeing the MSG retval = %d", retval);
       of_get_pkt_mbuf_by_of_msg_and_free(msg);
   }
   return OF_SUCCESS;
}

int32_t
handover_flow_removed_event_to_app(uint64_t datapath_handle,
                                   struct dprm_domain_entry *domain,
                                   uint8_t                   table_id,
                                   uint8_t                   msg_id,
                                   struct of_msg            *msg,
                                   struct ofl_flow_removed  *flow_removed_msg)
{
   struct of_app_hook_info  *app_entry=NULL;
   int32_t  retval = OF_ASYNC_MSG_PASS_TO_NEXT_APP;
   uint16_t offset = OF_APP_HOOK_LISTNODE_OFFSET;
   struct async_event_info *event_info_p =NULL;

   if (msg_id > OF_MAX_NUMBER_ASYNC_MSGS)
   {
      OF_LOG_MSG(OF_LOG_MOD, OF_LOG_ERROR,"%s:%d Invalid message with id=%d  \r\n",__FUNCTION__,__LINE__,msg_id);
      return OF_FAILURE;
   }


   if(get_ttprm_async_event_info(domain->ttp_name, table_id ,&event_info_p)!=OF_SUCCESS)
   {
     OF_LOG_MSG(OF_LOG_MOD, OF_LOG_DEBUG, "Get ttprm node failed!.");
     return  OF_FAILURE;
   }


   OF_LIST_SCAN(event_info_p[msg_id].app_list, app_entry, struct of_app_hook_info*, offset)
   {
     if(app_entry != NULL)
        retval = ((of_flow_removed_clbk_fn)(app_entry->call_back))(controller_handle,
                                                                   domain->domain_handle,
                                                                   datapath_handle,
                                                                   msg,
                                                                   flow_removed_msg,
                                                                   app_entry->cbk_arg1,
                                                                   app_entry->cbk_arg2);
     if(retval == OF_ASYNC_MSG_CONSUMED)
	     break;
     else if(retval == OF_ASYNC_MSG_PASS_TO_NEXT_APP)
	     continue;
     else if(retval == OF_ASYNC_MSG_DROP)
     {
	     //of_get_pkt_mbuf_by_of_msg_and_free(msg);
	     msg->desc.free_cbk(msg);
	     break;
     }
   }

   if(retval == OF_ASYNC_MSG_PASS_TO_NEXT_APP)
      // of_get_pkt_mbuf_by_of_msg_and_free(msg);
       msg->desc.free_cbk(msg);

   return OF_SUCCESS;
}



