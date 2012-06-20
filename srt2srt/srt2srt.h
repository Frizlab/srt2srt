/*
 *  srt2srt.h
 *  srt2srt
 *
 *  Created by François LAMBOLEY on 4/25/11.
 *  Copyright 2011 Frost Land. All rights reserved.
 */

#ifndef SRT_2_SRT_H_
# define SRT_2_SRT_H_

# include <stdio.h>
# include "constants.h"

# define BUFF_SIZE (1024)

/* We will implement an engine state algorithm */
typedef struct s_engine_datas {
	FILE *out_fp;
	const t_srt2srt_options *options;
	
	BOOL parsing_start_time;
	unsigned long int cur_parsed_number;
	unsigned long int cur_parsed_time_seconds;
	
	char prev_char;
} t_engine_datas;
typedef BOOL (*f_engine)(char c, t_engine_datas *datas, void/*f_engine*/ *engine);

void print_time(unsigned long int time_seconds, unsigned long int seconds_fraction, FILE *out_fp);
void compute_time_diff_with(unsigned long int *time_seconds, unsigned long int *seconds_fraction, const t_srt2srt_options *options);

/* in_file:  pass NULL for stdin
 * out_file: pass NULL for stdout */
t_error treat_srt(const t_srt2srt_options *options, BOOL print_error_messages);

#endif /* !SRT_2_SRT_H_ */
