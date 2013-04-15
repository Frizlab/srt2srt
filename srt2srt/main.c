/*
 *  main.c
 *  srt2srt
 *
 *  Created by François LAMBOLEY on 4/24/11.
 *  Copyright 2011 Frost Land. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <assert.h>
#include <strings.h>

#include "srt2srt.h"
#include "constants.h"
#include "free_const_char.h"

t_error usage(const char *progname, BOOL from_syntax_error) {
	FILE *out = stdout;
	if (from_syntax_error) out = stderr;
	
	fprintf(out, "usage: %s [option ...] filename\n", progname);
	fprintf(out, "If filename is \"-\", stdin is taken.\n");
	fprintf(out, "\nOptions:\n");
	fprintf(out, "\n   -d ms   --delay-before=ms\n   Sets the delay of the output srt file in millisecond. Must be an integer, but can be negative. This delay is applied before the fps adjustements.\n");
	fprintf(out, "\n   -e ms   --delay-after=ms\n   Sets the delay of the output srt file in millisecond. Must be an integer, but can be negative. This delay is applied after the fps adjustements.\n");
	fprintf(out, "\n   -i fps   --in-fps=fps\n   Sets the input fps of the given srt file. Default to 25.\n");
	fprintf(out, "\n   -o fps   --out-fps=fps\n   Sets the output fps. Default to 23.976.\n");
	fprintf(out, "\n   -n   --normalize\n   Pushes time of overlapping subtitles to remove overlap.\n");
	fprintf(out, "\n   --output=filename\n   Sets the output of srt2srt. If omitted or set to \"-\", will output on stdout.\n");
	
	return from_syntax_error? SYNTAX_ERROR: NO_ERROR;
}

int main(int argc, char * const * argv) {
	int getopt_long_ret;
	t_error ret = NO_ERROR;
	BOOL parse_options_succeeded = YES;
	t_srt2srt_options options = {NO, NO, NULL, NULL, 0, 0, 25, 23.976};
	
	do {
		char *number_parse_check = NULL;
		struct option long_options[] =
		{
			/* These options set a flag. */
			{"verbose",   no_argument, &options.verbose,   YES},
			{"normalize", no_argument, &options.normalize, YES},
			/* These options don't set a flag.
			 We distinguish them by their indices. */
			{"help",         no_argument,       NULL, 'h'},
			{"delay-before", required_argument, NULL, 'd'},
			{"delay-after",  required_argument, NULL, 'e'},
			{"in-fps",       required_argument, NULL, 'i'},
			{"out-fps",      required_argument, NULL, 'o'},
			{"output",       required_argument, NULL, 'p'},
			{0, 0, 0, 0}
		};
		
		/* getopt_long stores the option index here. */
		int option_index = 0;
		getopt_long_ret = getopt_long(argc, argv, "vnhd:e:i:o:p:", long_options, &option_index);
		
		switch (getopt_long_ret) {
			case -1: break; /* End of options */
			case 0:
				/* If this option set a flag, it is here the verbose option and
				 * there is nothing else to do. */
				if (long_options[option_index].flag != NULL) break;
				
				/* All options that don't set a flag should return an indice
				 * (see the definition of the long_options array). */
				assert(0);
				break;
			case 'p':
				if (strcmp(optarg, "-") != 0) {
					char *optarg_copy = malloc(sizeof(char)*strlen(optarg));
					strcpy(optarg_copy, optarg);
					options.output = optarg_copy;
				}
				break;
			case 'v':
				options.verbose = YES;
				break;
			case 'n':
				options.normalize = YES;
				break;
			case 'h':
				return usage(argv[0], NO);
				break;
			case 'd':
				/* We could use atoi instead of having to make a dirty cast,
				 * but I specifially want to check that the number is correctly
				 * parsed, and atoi cannot this.
				 * Moreover, atoi has been deprecated by strtol (man atoi) */
				options.delay_before = (int)strtol(optarg, &number_parse_check, 10);
				if (*number_parse_check != '\0') {
					fprintf(stderr, "%s: bad argument for the \"delay-before\" option.\n", argv[0]);
					parse_options_succeeded = NO;
				}
				break;
			case 'e':
				options.delay_after = (int)strtol(optarg, &number_parse_check, 10);
				if (*number_parse_check != '\0') {
					fprintf(stderr, "%s: bad argument for the \"delay-after\" option.\n", argv[0]);
					parse_options_succeeded = NO;
				}
				break;
			case 'i':
				options.ifps = strtof(optarg, &number_parse_check);
				if (*number_parse_check != '\0' || options.ifps <= 0) {
					fprintf(stderr, "%s: bad argument for the \"in-fps\" option.\n", argv[0]);
					parse_options_succeeded = NO;
				}
				break;
			case 'o':
				options.ofps = strtof(optarg, &number_parse_check);
				if (*number_parse_check != '\0' || options.ofps <= 0) {
					fprintf(stderr, "%s: bad argument for the \"out-fps\" option.\n", argv[0]);
					parse_options_succeeded = NO;
				}
				break;
			case '?':
				parse_options_succeeded = NO;
				break; /* getopt_long already printed an error message. */
			default:
				assert(0);
		}
	} while (getopt_long_ret != -1 && parse_options_succeeded);
	
	if (!parse_options_succeeded || optind != argc - 1) {
		ret = usage(argv[0], YES);
		goto end;
	}
	
	if (strcmp(argv[argc - 1], "-") != 0) {
		char *arg_copy = malloc(sizeof(char)*(strlen(argv[argc - 1]) + 1));
		strcpy(arg_copy, argv[argc - 1]);
		options.input = arg_copy;
	}
	
	if (options.verbose) {
		fprintf(stderr, "Easter egg: verbose mode is activated!\n");
		fprintf(stderr, "Options:\n");
		fprintf(stderr, "   normalize:    %s\n", options.normalize? "on": "off");
		fprintf(stderr, "   delay-before: %d\n", options.delay_before);
		fprintf(stderr, "   delay-after:  %d\n", options.delay_after);
		fprintf(stderr, "   in-fps:       %g\n", options.ifps);
		fprintf(stderr, "   out-fps:      %g\n", options.ofps);
		fprintf(stderr, "   output:       %s\n", options.output != NULL? options.output: "stdout");
		fprintf(stderr, "Treated file is: %s\n", options.input != NULL? options.input: "stdin");
	}
	
	ret = treat_srt(&options, YES);
	
end:
	if (options.input != NULL)  free_const_char(options.input);
	if (options.output != NULL) free_const_char(options.output);
	return ret;
}
