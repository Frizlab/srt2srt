/*
 *  srt2srt.c
 *  srt2srt
 *
 *  Created by François LAMBOLEY on 4/25/11.
 *  Copyright 2011 Frost Land. All rights reserved.
 */

#include "srt2srt.h"

#include <ctype.h>

BOOL wait_for_srt_entry_idx_start(char c, t_engine_datas *datas, void *engine);
BOOL wait_for_srt_entry_idx_end(char c, t_engine_datas *datas, void *engine);
BOOL wait_for_srt_entry_time_hour_start(char c, t_engine_datas *datas, void *engine);
BOOL wait_for_srt_entry_time_hour_end(char c, t_engine_datas *datas, void *engine);
BOOL wait_for_srt_entry_time_minute_end(char c, t_engine_datas *datas, void *engine);
BOOL wait_for_srt_entry_time_second_end(char c, t_engine_datas *datas, void *engine);
BOOL wait_for_srt_entry_time_fraction_end(char c, t_engine_datas *datas, void *engine);
BOOL wait_for_srt_entry_arrow_start(char c, t_engine_datas *datas, void *engine);
BOOL wait_for_srt_entry_arrow_middle(char c, t_engine_datas *datas, void *engine);
BOOL wait_for_srt_entry_arrow_end(char c, t_engine_datas *datas, void *engine);
BOOL wait_for_blank_line(char c, t_engine_datas *datas, void *engine);

void compute_time_diff_with(unsigned long int *time_seconds, unsigned long int *seconds_fraction, const t_srt2srt_options *options) {
	double time_total_seconds = *seconds_fraction;
	
	while (time_total_seconds > 1.) time_total_seconds /= 10;
	time_total_seconds += *time_seconds;
	
	/* Doing the actual time modification here */
	time_total_seconds -= ((double)options->delay_before / 1000.);
	time_total_seconds *= options->ifps;
	time_total_seconds /= options->ofps;
	time_total_seconds -= ((double)options->delay_after / 1000.);
	/* Done */
	
	*time_seconds = time_total_seconds;
	*seconds_fraction = (time_total_seconds - *time_seconds) * 1000;
}

void print_time(unsigned long int time_seconds, unsigned long int seconds_fraction, FILE *out_fp) {
	unsigned long int hours, minutes;
	
	hours = time_seconds/(60*60);
	time_seconds -= hours * 60 * 60;
	minutes = time_seconds/60;
	time_seconds -= minutes * 60;
	
	fprintf(out_fp, "%02ld:%02ld:%02ld", hours, minutes, time_seconds);
	fprintf(out_fp, ",%ld", seconds_fraction);
}

BOOL wait_for_srt_entry_idx_start(char c, t_engine_datas *datas, void *engine) {
	if (datas->options->verbose) fprintf(stderr, "Entering wait_for_srt_entry_idx_start; char is \"%c\".\n", c);
	
	if (isprint(c) && !isdigit(c) && c != ' ') return NO;
	putc(c, datas->out_fp);
	
	if (isdigit(c)) *(f_engine*)engine = &wait_for_srt_entry_idx_end;
	
	return YES;
}

BOOL wait_for_srt_entry_idx_end(char c, t_engine_datas *datas, void *engine) {
	if (datas->options->verbose) fprintf(stderr, "Entering wait_for_srt_entry_idx_end; char is \"%c\".\n", c);
	
	if (c == '\r' || c == '\n') {
		putc(c, datas->out_fp);
		
		datas->parsing_start_time = YES;
		*(f_engine*)engine = &wait_for_srt_entry_time_hour_start;
		return YES;
	}
	
	if (!isdigit(c)) return NO;
	putc(c, datas->out_fp);
	
	return YES;
}

BOOL wait_for_srt_entry_time_hour_start(char c, t_engine_datas *datas, void *engine) {
	if (datas->options->verbose) fprintf(stderr, "Entering wait_for_srt_entry_time_hour_start; char is \"%c\".\n", c);
	
	if (isprint(c) && !isdigit(c) && c != ' ') return NO;
	
	if (!isdigit(c)) putc(c, datas->out_fp);
	else {
		datas->cur_parsed_number = c - '0';
		*(f_engine*)engine = &wait_for_srt_entry_time_hour_end;
	}
	
	return YES;
}

BOOL wait_for_srt_entry_time_hour_end(char c, t_engine_datas *datas, void *engine) {
	if (datas->options->verbose) fprintf(stderr, "Entering wait_for_srt_entry_time_hour_end; char is \"%c\".\n", c);
	
	if (c == ':') {
		datas->cur_parsed_time_seconds = datas->cur_parsed_number * 60 * 60;
		datas->cur_parsed_number = 0;
		
		*(f_engine*)engine = &wait_for_srt_entry_time_minute_end;
		return YES;
	}
	
	if (!isdigit(c)) return NO;
	datas->cur_parsed_number *= 10;
	datas->cur_parsed_number += c - '0';
	
	return YES;
}

BOOL wait_for_srt_entry_time_minute_end(char c, t_engine_datas *datas, void *engine) {
	if (datas->options->verbose) fprintf(stderr, "Entering wait_for_srt_entry_time_minute_end; char is \"%c\".\n", c);
	
	if (c == ':') {
		datas->cur_parsed_time_seconds += datas->cur_parsed_number * 60;
		datas->cur_parsed_number = 0;
		
		*(f_engine*)engine = &wait_for_srt_entry_time_second_end;
		return YES;
	}
	
	if (!isdigit(c)) return NO;
	datas->cur_parsed_number *= 10;
	datas->cur_parsed_number += c - '0';
	
	return YES;
}

BOOL wait_for_srt_entry_time_second_end(char c, t_engine_datas *datas, void *engine) {
	if (datas->options->verbose) fprintf(stderr, "Entering wait_for_srt_entry_time_second_end; char is \"%c\".\n", c);
	
	if (c == ',') {
		datas->cur_parsed_time_seconds += datas->cur_parsed_number;
		datas->cur_parsed_number = 0;
		
		*(f_engine*)engine = &wait_for_srt_entry_time_fraction_end;
		return YES;
	}
	
	if (!isdigit(c)) return NO;
	datas->cur_parsed_number *= 10;
	datas->cur_parsed_number += c - '0';
	
	return YES;
}

BOOL wait_for_srt_entry_time_fraction_end(char c, t_engine_datas *datas, void *engine) {
	if (datas->options->verbose) fprintf(stderr, "Entering wait_for_srt_entry_time_fraction_end; char is \"%c\".\n", c);
	
	if (( datas->parsing_start_time && (isblank(c) || c == '-')) ||
		 (!datas->parsing_start_time && (isblank(c) || c == '\n' || c == '\r'))) {
		compute_time_diff_with(&datas->cur_parsed_time_seconds, &datas->cur_parsed_number, datas->options);
		print_time(datas->cur_parsed_time_seconds, datas->cur_parsed_number, datas->out_fp);
		
		putc(c, datas->out_fp);
		datas->cur_parsed_number = 0;
		
		if (!datas->parsing_start_time) *(f_engine*)engine = &wait_for_blank_line;
		else {
			if (c != '-') *(f_engine*)engine = &wait_for_srt_entry_arrow_start;
			else          *(f_engine*)engine = &wait_for_srt_entry_arrow_middle;
		}
		return YES;
	}
	
	if (!isdigit(c)) return NO;
	datas->cur_parsed_number *= 10;
	datas->cur_parsed_number += c - '0';
	
	return YES;
}

BOOL wait_for_srt_entry_arrow_start(char c, t_engine_datas *datas, void *engine) {
	if (datas->options->verbose) fprintf(stderr, "Entering wait_for_srt_entry_arrow_start; char is \"%c\".\n", c);
	
	putc(c, datas->out_fp);
	
	if (c == '-') {
		*(f_engine*)engine = &wait_for_srt_entry_arrow_middle;
		return YES;
	}
	
	return isblank(c);
}

BOOL wait_for_srt_entry_arrow_middle(char c, t_engine_datas *datas, void *engine) {
	if (datas->options->verbose) fprintf(stderr, "Entering wait_for_srt_entry_arrow_middle; char is \"%c\".\n", c);
	
	putc(c, datas->out_fp);
	
	if (c == '-') {
		*(f_engine*)engine = &wait_for_srt_entry_arrow_end;
		return YES;
	}
	
	return NO;
}

BOOL wait_for_srt_entry_arrow_end(char c, t_engine_datas *datas, void *engine) {
	if (datas->options->verbose) fprintf(stderr, "Entering wait_for_srt_entry_arrow_end; char is \"%c\".\n", c);
	
	putc(c, datas->out_fp);
	
	if (c == '>') {
		datas->parsing_start_time = NO;
		*(f_engine*)engine = &wait_for_srt_entry_time_hour_start;
		return YES;
	}
	
	return NO;
}

BOOL wait_for_blank_line(char c, t_engine_datas *datas, void *engine) {
	if (datas->options->verbose) fprintf(stderr, "Entering wait_for_blank_line; char is \"%c\".\n", c);
	
	putc(c, datas->out_fp);
	
	if ((datas->prev_char == '\n' && c == '\n') ||
		 (datas->prev_char == '\n' && c == '\r') ||
		 (datas->prev_char == '\r' && c == '\r')) {
		datas->prev_char = '\0';
		*(f_engine*)engine = &wait_for_srt_entry_idx_start;
		
		return YES;
	}
	
	datas->prev_char = c;
	
	return YES;
}

t_error treat_srt(const t_srt2srt_options *options, BOOL print_error_messages) {
	t_error ret = NO_ERROR;
	FILE *fpi = stdin, *fpo = stdout;
	
	char buff[BUFF_SIZE];
	size_t true_buff_size, cur_buff_pos;
	
	BOOL error = NO;
	f_engine engine = &wait_for_srt_entry_idx_start;
	t_engine_datas engine_datas = {fpo, options, YES, 0, 0, '\0'};
	
	if (options->input != NULL) {
		fpi = fopen(options->input, "rb");
		if (fpi == NULL && print_error_messages) perror("Cannot open input");
		if (fpi == NULL) {ret = OPEN_ERROR; goto end;}
	}
	if (options->output != NULL) {
		fpo = fopen(options->output, "wb");
		if (fpo == NULL && print_error_messages) perror("Cannot open output");
		if (fpo == NULL) {ret = OPEN_ERROR; goto end;}
	}
	
	while ((true_buff_size = fread(buff, sizeof(char), BUFF_SIZE, fpi)) != 0 && !error)
		for (cur_buff_pos = 0; cur_buff_pos < true_buff_size && !error; ++cur_buff_pos)
			error = !engine(buff[cur_buff_pos], &engine_datas, &engine);
	
	if (error) fprintf(stderr, "Unrecoverable error while parsing the input file.\nThe output may not be complete.\n");
	
end:
	if (fpi != NULL && fpi != stdin)  fclose(fpi);
	if (fpo != NULL && fpo != stdout) fclose(fpo);
	return ret;
}
