/*	$NetBSD: histedit.h,v 1.41 2009/09/07 21:24:33 christos Exp $	*/

/*-
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Christos Zoulas of Cornell University.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)histedit.h	8.2 (Berkeley) 1/3/94
 */

/*
 * histedit.h: Line editor and history_t interface.
 */
#ifndef _HISTEDIT_H_
#define	_HISTEDIT_H_

#define	LIBEDIT_MAJOR 2
#define	LIBEDIT_MINOR 11

#include <sys/types.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ==== Editing ====
 */

typedef struct editline edit_line;

/*
 * For user-defined function interface
 */
typedef struct lineinfo {
	const char	*buffer;
	const char	*cursor;
	const char	*lastchar;
} line_info;

/*
 * edit_line editor function return codes.
 * For user-defined function interface
 */
#define	CC_NORM		0
#define	CC_NEWLINE	1
#define	CC_EOF		2
#define	CC_ARGHACK	3
#define	CC_REFRESH	4
#define	CC_CURSOR	5
#define	CC_ERROR	6
#define	CC_FATAL	7
#define	CC_REDISPLAY	8
#define	CC_REFRESH_BEEP	9

/*
 * Initialization, cleanup, and resetting
 */
edit_line	*el_init(const char *, FILE *, FILE *, FILE *);
void		 el_end(edit_line *);
void		 el_reset(edit_line *);

/*
 * Get a line, a character or push a string back in the input queue
 */
const char	*el_gets(edit_line *, int *);
int		 el_getc(edit_line *, char *);
void		 el_push(edit_line *, const char *);

/*
 * Beep!
 */
void		 el_beep(edit_line *);

/*
 * High level function internals control
 * Parses argc, argv array and executes builtin editline commands
 */
int		 el_parse(edit_line *, int, const char **);

/*
 * Low level editline access functions
 */
int		 el_set(edit_line *, int, ...);
int		 el_get(edit_line *, int, ...);
unsigned char	_el_fn_complete(edit_line *, int);

/*
 * el_set/el_get parameters
 */
#define	EL_PROMPT	0	/* , el_pfunc_t);		*/
#define	EL_TERMINAL	1	/* , const char *);		*/
#define	EL_EDITOR	2	/* , const char *);		*/
#define	EL_SIGNAL	3	/* , int);			*/
#define	EL_BIND		4	/* , const char *, ..., NULL);	*/
#define	EL_TELLTC	5	/* , const char *, ..., NULL);	*/
#define	EL_SETTC	6	/* , const char *, ..., NULL);	*/
#define	EL_ECHOTC	7	/* , const char *, ..., NULL);	*/
#define	EL_SETTY	8	/* , const char *, ..., NULL);	*/
#define	EL_ADDFN	9	/* , const char *, const char *	*/
				/* , el_func_t);		*/
#define	EL_HIST		10	/* , hist_fun_t, const char *);	*/
#define	EL_EDITMODE	11	/* , int);			*/
#define	EL_RPROMPT	12	/* , el_pfunc_t);		*/
#define	EL_GETCFN	13	/* , el_rfunc_t);		*/
#define	EL_CLIENTDATA	14	/* , void *);			*/
#define	EL_UNBUFFERED	15	/* , int);			*/
#define	EL_PREP_TERM	16	/* , int);			*/
#define	EL_GETTC	17	/* , const char *, ..., NULL);	*/
#define	EL_GETFP	18	/* , int, FILE **);		*/
#define	EL_SETFP	19	/* , int, FILE *);		*/
#define	EL_REFRESH	20	/* , void);			*/
#define	EL_PROMPT_ESC	21	/* , el_pfunc_t, char);		*/
#define	EL_RPROMPT_ESC	22	/* , el_pfunc_t, char);		*/

#define	EL_BUILTIN_GETCFN	(NULL)

/*
 * Source named file or $PWD/.editrc or $HOME/.editrc
 */
int		el_source(edit_line *, const char *);

/*
 * Must be called when the terminal changes size; If EL_SIGNAL
 * is set this is done automatically otherwise it is the responsibility
 * of the application
 */
void		 el_resize(edit_line *);

/*
 * User-defined function interface.
 */
const line_info	*el_line(edit_line *);
int		 el_insertstr(edit_line *, const char *);
void		 el_deletestr(edit_line *, int);


/*
 * ==== history_t ====
 */

typedef struct history history_t;

typedef struct hist_event {
	int		 num;
	const char	*str;
} hist_event;

/*
 * history_t access functions.
 */
history_t *	history_init(void);
void		history_end(history_t *);

int		history(history_t *, hist_event *, int, ...);

#define	H_FUNC		 0	/* , UTSL		*/
#define	H_SETSIZE	 1	/* , const int);	*/
#define	H_GETSIZE	 2	/* , void);		*/
#define	H_FIRST		 3	/* , void);		*/
#define	H_LAST		 4	/* , void);		*/
#define	H_PREV		 5	/* , void);		*/
#define	H_NEXT		 6	/* , void);		*/
#define	H_CURR		 8	/* , const int);	*/
#define	H_SET		 7	/* , int);		*/
#define	H_ADD		 9	/* , const char *);	*/
#define	H_ENTER		10	/* , const char *);	*/
#define	H_APPEND	11	/* , const char *);	*/
#define	H_END		12	/* , void);		*/
#define	H_NEXT_STR	13	/* , const char *);	*/
#define	H_PREV_STR	14	/* , const char *);	*/
#define	H_NEXT_EVENT	15	/* , const int);	*/
#define	H_PREV_EVENT	16	/* , const int);	*/
#define	H_LOAD		17	/* , const char *);	*/
#define	H_SAVE		18	/* , const char *);	*/
#define	H_CLEAR		19	/* , void);		*/
#define	H_SETUNIQUE	20	/* , int);		*/
#define	H_GETUNIQUE	21	/* , void);		*/
#define	H_DEL		22	/* , int);		*/
#define	H_NEXT_EVDATA	23	/* , const int, histdata_t *);	*/
#define	H_DELDATA	24	/* , int, histdata_t *);*/
#define	H_REPLACE	25	/* , const char *, histdata_t);	*/



/*
 * ==== Tokenization ====
 */

typedef struct tokenizer tokenizer_t;

/*
 * String tokenization functions, using simplified sh(1) quoting rules
 */
tokenizer_t	*tok_init(const char *);
void		 tok_end(tokenizer_t *);
void		 tok_reset(tokenizer_t *);
int		 tok_line(tokenizer_t *, const line_info *,
		    int *, const char ***, int *, int *);
int		 tok_str(tokenizer_t *, const char *,
		    int *, const char ***);

#ifdef __cplusplus
}
#endif

#endif /* _HISTEDIT_H_ */
