#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "token.h"

int 
main(int argc, char **argv)
{
    char *ptable[MAXCOL];       /* array of pointers to column starts */
    char buf[BUFSZ];            /* input buffer */       
    unsigned long linecnt = 0;  /* input line number (record number) counter */
    unsigned long dropcnt = 0;  /* number of input lines dropped counter */
    int cols = 0;               /* number of columns in the input */
    char *ptr;                  /* pointer for use by strtol() */


    int colPos = 0;
	if (argc == 2 || argc == 3) {
		if (strncmp(*(argv + 1), "-c", 2) != 0) {
			fprintf(stderr, "Usage: %s -c #\n", *argv);
			return EXIT_FAILURE;
		}
		if (argc == 2) {
			colPos = 1;
		}
		else {
			colPos = 2;
		}
	}
	else {
		fprintf(stderr, "Usage: %s -c #\n", *argv);
		return EXIT_FAILURE;
	}
    	errno = 0;
    	if (colPos == 1) {
		cols = (int)strtol(*(argv + colPos) + 2, &ptr, 10);
	}
	else {
		cols = (int)strtol(*(argv + colPos), &ptr, 10);
	}
	if ((*ptr != '\0') || (errno != 0)) {
		fprintf(stderr, "%s: improper column value\n", *argv);
		return EXIT_FAILURE;
	}
	if (cols < MINCOL || cols > MAXCOL) {
		fprintf(stderr, "%s: column value %d, out of range, min %d max is %d\n", *argv, cols, MINCOL, MAXCOL);
		return EXIT_FAILURE;
	}
    	/*
     	* read the input one line at a time, break into tokens and writse out the
     	* selected columns in a debugging format
     	*/
    	while (fgets(buf, BUFSZ, stdin)  != NULL) {
      		linecnt++;             /* count the records *
        	/*
         	* breaks the input into columns (data fields)
         	* uses INDELIM as the delimiter
        	*/
        	if (token(buf, INDELIM, cols, ptable, linecnt, *argv) != 0) {
            		dropcnt++;	/* count the dropped rows */
		}
       	 	else {
           		/*
            		* token returned ok, prints out columns one at a time
            		*/
            		for (int i = 0; i < cols; i++) {
	    			printf("Line[%lu],Column[%d]:%s\n", linecnt, i + 1, *(ptable + i));
	    		}
        	}
    	}
    	fprintf(stderr, "%s: %lu records input, %lu dropped\n", *argv, linecnt, dropcnt);
    	if (dropcnt > 0) {
        	return EXIT_FAILURE;
	}
    	return EXIT_SUCCESS;
}
