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

#ifndef __CNTLR_SYSTEM_H
#define __CNTLR_SYSTEM_H

/* Applications shared library directory path*/
#define CNTLR_DIR                "/usr/local/ondir_platform/2.0/"
#define CNTLR_SHARED_APP_DIR     "/lib/apps"
#define CNTLR_SHARED_SUBTTP_DIR     "/lib/subttp"
#define CNTLR_SHARED_APID_DIR     "/lib/apid"
#define CNTLR_SHARED_LIM_DIR     "/lib/lim"
#define CNTLR_SHARED_MAINTTP_DIR     "/lib/mainttp"
#define CNTLR_SHARED_INFRA_DIR     "/lib/infra"
#define CNTLR_SHARED_UTILS_DIR     "/lib/utils"
#define CNTLR_SHARED_MGMT_DIR     "/lib/mgmt"
#define CNTLR_MAX_DIR_PATH_LEN   128

/*DPRM related resource definitions*/
#define DPRM_MAX_SWITCH_ENTRIES              64
#define DPRM_MAX_DOMAIN_ENTRIES              64
#define DPRM_MAX_DATAPATH_ENTRIES           128

#define DPRM_MAX_SWNODE_DPTBL_ENTRIES       128
#define DPRM_MAX_DMNODE_DPTBL_ENTRIES       128
#define DPRM_MAX_DMNODE_OFTABLE_ENTRIES      48
#define DPRM_MAX_DPNODE_PORTTBL_ENTRIES     128

#define DPRM_MAX_NOTIFICATION_ENTRIES       128
#define DPRM_MAX_ATTRIBUTE_ENTRIES          128

/* How many of them to be pre allocated.Remaining will be allocated from heap in run time */
#define DPRM_MAX_SWITCH_STATIC_ENTRIES         16 
#define DPRM_MAX_DOMAIN_STATIC_ENTRIES         16
#define DPRM_MAX_DATAPATH_STATIC_ENTRIES       32

#define DPRM_MAX_SWNODE_DPTBL_STATIC_ENTRIES   32
#define DPRM_MAX_DMNODE_DPTBL_STATIC_ENTRIES   32
#define DPRM_MAX_DMNODE_OFTABLE_STATIC_ENTRIES 32
#define DPRM_MAX_DPNODE_PORTTBL_STATIC_ENTRIES 32

#define DPRM_MAX_NOTIFICATION_STATIC_ENTRIES   32 
#define DPRM_MAX_ATTRIBUTE_STATIC_ENTRIES      32 

/*CRM related resource definitions*/
#define CRM_MAX_VN_ENTRIES                  64
#define CRM_MAX_SUBNET_ENTRIES              64
#define CRM_MAX_VM_ENTRIES                  64
#define CRM_MAX_TENANT_ENTRIES              64
#define CRM_MAX_COMPUTE_NODES_ENTRIES       64
#define CRM_MAX_VN_PORT_ENTRIES             64
#define CRM_MAX_VM_PORT_ENTRIES             64
#define CRM_MAX_PORT_NODES_ENTRIES          64
#define CRM_MAX_NOTIFICATION_ENTRIES        64
#define CRM_MAX_ATTRIBUTE_ENTRIES           64
#define OF_FC_MAX_VIEW_NODE_ENTRIES         64
#define CRM_MAX_VIEW_NODE_ENTRIES           64


/* How many of them to be pre allocated.Remaining will be allocated from heap in run time */
#define CRM_MAX_VM_STATIC_ENTRIES               16 
#define CRM_MAX_SUBNET_STATIC_ENTRIES           16 
#define CRM_MAX_VN_STATIC_ENTRIES               16 
#define CRM_MAX_VM_STATIC_ENTRIES               16 
#define CRM_MAX_TENANT_STATIC_ENTRIES           16 
#define CRM_MAX_NOTIFICATION_STATIC_ENTRIES     16 
#define CRM_MAX_ATTRIBUTE_STATIC_ENTRIES        16
#define CRM_MAX_COMPUTE_NODES_STATIC_ENTRIES    16 
#define CRM_MAX_VN_PORT_STATIC_ENTRIES          16 
#define CRM_MAX_PORT_NODES_STATIC_ENTRIES       16 
#define OF_FC_MAX_VIEW_STATIC_NODE_ENTRIES      16
#define CRM_MAX_VIEW_STATIC_NODE_ENTRIES         16


/*NSRM related resource definition */
#define NSRM_MAX_NWSERVICE_OBJECT_ENTRIES                64
#define NSRM_MAX_NSCHAIN_OBJECT_ENTRIES                  64
#define NSRM_MAX_NSCHAIN_NWSERVICE_OBJECT_ENTRIES        64
#define NSRM_MAX_NWSERVICE_INSTANCE_ENTRIES              64
#define NSRM_MAX_NSCHAINSET_OBJECT_ENTRIES               64
#define NSRM_MAX_L2NW_SERVICE_MAP_ENTRIES                64
#define NSRM_MAX_SELECTION_RULE_ENTRIES                  64
#define NSRM_MAX_ZONE_ENTRIES                            64
#define NSRM_MAX_BYPASS_RULE_ENTRIES                     64
#define NSRM_MAX_SKIP_LIST_ENTRIES                       64
#define NSRM_MAX_NOTIFICATION_ENTRIES                    64
#define NSRM_MAX_ATTRIBUTE_ENTRIES                       64
#define NSRM_MAX_VIEW_NODE_ENTRIES                       64

/* How many of them to be pre allocated.Remaining will be allocated from heap in run time */
#define NSRM_MAX_NWSERVICE_OBJECT_STATIC_ENTRIES         16
#define NSRM_MAX_NSCHAIN_OBJECT_STATIC_ENTRIES           16
#define NSRM_MAX_NSCHAIN_NWSERVICE_OBJECT_STATIC_ENTRIES 16
#define NSRM_MAX_NWSERVICE_INSTANCE_STATIC_ENTRIES       16
#define NSRM_MAX_NSCHAINSET_OBJECT_STATIC_ENTRIES        16
#define NSRM_MAX_SELECTION_RULE_STATIC_ENTRIES           16
#define NSRM_MAX_ZONE_STATIC_ENTRIES           16
#define NSRM_MAX_BYPASS_RULE_STATIC_ENTRIES              16
#define NSRM_MAX_SKIP_LIST_STATIC_ENTRIES                16
#define NSRM_MAX_L2NW_SERVICE_MAP_STATIC_ENTRIES         16
#define NSRM_MAX_NOTIFICATION_STATIC_ENTRIES             16
#define NSRM_MAX_ATTRIBUTE_STATIC_ENTRIES                16
#define NSRM_MAX_VIEW_STATIC_NODE_ENTRIES                 64



#endif /*__CNTLR_SYSTEM_H*/
