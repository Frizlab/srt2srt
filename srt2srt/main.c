//
//  main.c
//  srt2srt
//
//  Created by François LAMBOLEY on 4/24/11.
//  Copyright 2011 Frost Land. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <assert.h>

typedef int BOOL;
#define NO  ((BOOL)0)
#define YES ((BOOL)1)

typedef enum e_error {
	NO_ERROR     = 0,
	SYNTAX_ERROR = 1
} t_error;

typedef struct s_prgm_options {
	BOOL verbose;	/* Default: NO */
	
	int delay;		/* Default: 0 */
	float ifps;		/* Default: 25 */
	float ofps;		/* Default: 23.976 */
} t_prgm_options;

t_error usage(const char *progname, BOOL from_syntax_error) {
	FILE *out = stdout;
	if (from_syntax_error) out = stderr;
	
	fprintf(out, "usage: %s [option ...] filename\n", progname);
	fprintf(out, "\n");
	fprintf(out, "Options:\n");
	fprintf(out, "\n   -d ms   --delay=ms\n   Sets the delay of the output srt file in millisecond. Must be an integer, but can be negative.\n");
	fprintf(out, "\n   -i fps   --in-fps=fps\n   Sets the input fps of the given srt file. Default to 25.\n");
	fprintf(out, "\n   -o fps   --out-fps=fps\n   Sets the output fps. Default to 23.976.\n");
	fprintf(out, "\nThe transformed file is outputted on stdout.\n");
	
	return from_syntax_error? SYNTAX_ERROR: NO_ERROR;
}

int main(int argc, char * const * argv) {
	int getopt_long_ret;
	t_prgm_options options = {NO, 0, 25, 23.976};
	
	do {
		char *number_parse_check = NULL;
		struct option long_options[] =
		{
			/* These options set a flag. */
			{"verbose", no_argument,       &options.verbose, YES},
			/* These options don't set a flag.
			 We distinguish them by their indices. */
			{"help",    no_argument,       NULL, 'h'},
			{"delay",   required_argument, NULL, 'd'},
			{"in-fps",  required_argument, NULL, 'i'},
			{"out-fps", required_argument, NULL, 'o'},
			{0, 0, 0, 0}
		};
		
		/* getopt_long stores the option index here. */
		int option_index = 0;
		getopt_long_ret = getopt_long(argc, argv, "hvo:d:i:", long_options, &option_index);
		
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
			case 'v':
				options.verbose = YES;
				break;
			case 'h':
				return usage(argv[0], NO);
				break;
			case 'd':
				/* We could use atoi instead of having to make a dirty cast,
				 * but I specifially want to check that the number is correctly
				 * parsed, and atoi cannot this.
				 * Moreover, atoi has been deprecated by strtol (man atoi) */
				options.delay = (int)strtol(optarg, &number_parse_check, 10);
				if (*number_parse_check != '\0') {
					fprintf(stderr, "%s: bad argument for the \"delay\" option.\n", argv[0]);
					return SYNTAX_ERROR;
				}
				break;
			case 'i':
				options.ifps = strtof(optarg, &number_parse_check);
				if (*number_parse_check != '\0' || options.ifps <= 0) {
					fprintf(stderr, "%s: bad argument for the \"in-fps\" option.\n", argv[0]);
					return SYNTAX_ERROR;
				}
				break;
			case 'o':
				options.ofps = strtof(optarg, &number_parse_check);
				if (*number_parse_check != '\0' || options.ofps <= 0) {
					fprintf(stderr, "%s: bad argument for the \"out-fps\" option.\n", argv[0]);
					return SYNTAX_ERROR;
				}
				break;
			case '?': break; /* getopt_long already printed an error message. */
			default:
				assert(0);
		}
	} while (getopt_long_ret != -1);
	
	if (optind != argc - 1)
		return usage(argv[0], YES);
	
	if (options.verbose) {
		printf("Easter egg: verbose mode is activated!\n");
		printf("Options:\n");
		printf("   delay:   %d\n", options.delay);
		printf("   in-fps:  %g\n", options.ifps);
		printf("   out-fps: %g\n", options.ofps);
		printf("Treated file path is: %s\n", argv[argc - 1]);
	}
	
	return NO_ERROR;
}
