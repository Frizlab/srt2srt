/*
 *  srt2srt.c
 *  srt2srt
 *
 *  Created by François LAMBOLEY on 4/25/11.
 *  Copyright 2011 Frost Land. All rights reserved.
 */

#include "srt2srt.h"

#include <stdio.h>

t_error treat_srt(const t_srt2srt_options *options, BOOL print_error_messages) {
	t_error ret = NO_ERROR;
	FILE *fpi = stdin, *fpo = stdout;
	
	if (options->input != NULL) {
		fpi = fopen(options->input, "r");
		if (fpi == NULL && print_error_messages) perror("Cannot open input");
		if (fpi == NULL) {ret = OPEN_ERROR; goto end;}
	}
	if (options->output != NULL) {
		fpo = fopen(options->output, "w");
		if (fpo == NULL && print_error_messages) perror("Cannot open output");
		if (fpo == NULL) {ret = OPEN_ERROR; goto end;}
	}
	
	
	
end:
	if (fpi != NULL && fpi != stdin)  fclose(fpi);
	if (fpo != NULL && fpo != stdout) fclose(fpo);
	return ret;
}
