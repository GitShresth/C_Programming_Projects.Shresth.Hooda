#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "token.h"

/*
 * token
 * 
 * usage:   input buffer in dsv format, builds table of pointers to each field
 * args:
 *  buf     DSV data record stored in an array of characters, '\0' terminated
 *  delim   the field delimiter             
 *  cnt     number of data fields that a proper data record has
 *  ptable  points at array of pointers each element points at a data field
 *  lineno  line number of this record for printing error messages
 *  argv0   name of program used when printing errors
 * returns:
 *          0 if no errors founds
 *         -1 otherwise
 */
int
token(char *buf, char delim, int cnt, char **ptable, unsigned long lineno,
            char *argv0)
{	
	int bufPos = 0;			/*Position in buffer*/
	char *bufEnd = buf + 30;        /*The end of the while loop argument*/
	int fieldsNum = 0;		/*The number of data entries*/

	for(int i = 0; i < cnt + 1; i++) {
		if (i == 0) {
			*ptable = buf;
			fieldsNum++;
		}
		else {
			while(buf < bufEnd) {	
				if (*(buf + bufPos) == '\"') {	
					fprintf(stderr, "%s: drop line %lu, \" in unquoted field\n", argv0, lineno);
					return -1;
				}
				if (*(buf + bufPos) == delim) {
					if(i == cnt) {
						fprintf(stderr, "%s: drop line %lu, too many columns\n", argv0, lineno);
						return -1;
					}
					fieldsNum++;
					*(buf + bufPos) = '\0';
					bufPos++;
					*(ptable + i) = (buf + bufPos);
					break;
				}
				if (*(buf + bufPos) == '\n') {
					*(buf + bufPos) = '\0';
					i = cnt + 1;
					break;
				}
			bufPos++;
			}	
		}
	}	
	if (fieldsNum < cnt) {
		fprintf(stderr, "%s: drop line %lu, too few columns\n", argv0, lineno);
		return -1;
	}

	if (*(buf + bufPos) == '\"') {	
		fprintf(stderr, "%s: drop line %lu, \" in unquoted field\n", argv0, lineno);
		return -1;	
	}
	return 0;
}	


