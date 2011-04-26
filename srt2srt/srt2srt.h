/*
 *  srt2srt.h
 *  srt2srt
 *
 *  Created by François LAMBOLEY on 4/25/11.
 *  Copyright 2011 Frost Land. All rights reserved.
 */

#ifndef SRT_2_SRT_H_
#define SRT_2_SRT_H_

#include "constants.h"

/* in_file:  pass NULL for stdin
 * out_file: pass NULL for stdout */
t_error treat_srt(const t_srt2srt_options *options, BOOL print_error_messages);

#endif /* !SRT_2_SRT_H_ */
