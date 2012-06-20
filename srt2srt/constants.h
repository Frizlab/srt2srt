/*
 *  constants.h
 *  srt2srt
 *
 *  Created by François LAMBOLEY on 4/25/11.
 *  Copyright 2011 Frost Land. All rights reserved.
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

typedef int BOOL;
#define NO  ((BOOL)0)
#define YES ((BOOL)1)

typedef enum e_error {
	NO_ERROR     = 0,
	SYNTAX_ERROR = 1,
	OPEN_ERROR   = 2
} t_error;

typedef struct s_srt2srt_options {
	BOOL verbose;       /* Default: NO */
	
	const char *input;  /* No default, must be set. */
	const char *output; /* Default: NULL. Means stdout. */
	
	int delay_before;   /* Default: 0 */
	int delay_after;    /* Default: 0 */
	float ifps;         /* Default: 25 */
	float ofps;         /* Default: 23.976 */
} t_srt2srt_options;

#endif /* !CONSTANTS_H_ */
