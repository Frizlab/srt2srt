//
//  main.c
//  srt2srt
//
//  Created by François LAMBOLEY on 4/24/11.
//  Copyright 2011 Frost Land. All rights reserved.
//

#include <stdio.h>

typedef int BOOL;

typedef enum e_error {
	NO_ERROR     = 0,
	SYNTAX_ERROR = 1
} t_error;

t_error usage(const char *progname, BOOL from_syntax_error) {
	FILE *out = stdout;
	if (from_syntax_error) out = stderr;
	
	fprintf(out, "Usage: %s ...", progname);
	
	return from_syntax_error? SYNTAX_ERROR: NO_ERROR;
}

int main(int argc, const char * argv[]) {
	return NO_ERROR;
}
