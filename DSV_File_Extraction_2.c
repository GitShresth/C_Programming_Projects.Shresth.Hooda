#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "token.h"

/*
 *  token
 * 
 *  usage:   input buffer in dsv format, builds table of pointers to each field
 *  args:
 *  buf     DSV data record stored in an array of characters, '\0' terminated
 *  delim   the field delimiter             
 *  cnt     number of data fields that a proper data record has
 *  ptable  points at array of pointers each element points at a data field
 *  lineno  line number of this record for printing error messages
 *  argv0   name of program used when printing errors
 *  returns:
 *           0 if no errors founds
 *          -1 otherwise
 */

int
token(char *buf, char delim, int cnt, char **ptable, unsigned long lineno, char *argv0)
{	
	int bufPos = 0;			/*Position in buffer*/
	int lastIndex = 0;
	char *bufEnd = buf + 30;        /*End of the while loop argument*/
	int fieldsNum = 0;		/*Number of data entries*/
	int quoted = 0;
	int qState = 0;
	
	for(int i = 0; i < cnt + 1; i++) {
		int endQuotePos = 0;
		int startBufPos = bufPos;
		quoted = 0;
		if(*(buf+bufPos) == '\"') {
			int qPos = bufPos + 1;
			while(*(buf+qPos) != '\n') {
				if(*(buf+qPos) == '\"') {
					if(*(buf+1+qPos) == delim || *(buf+1+qPos) == '\n') {
						if(*(buf-1+qPos) != '\"' || (qPos == bufPos + 1 && *(buf-1+qPos) == '\"')) {
							quoted = 1;
							endQuotePos = qPos;
							break;
						}		
					}		
				}
			qPos++;
			if(qPos == 40) {
				break;
			}
			}
			if(quoted == 0) {
				fprintf(stderr, "%s: drop line %lu, quoted field not terminated\n", argv0, lineno);
				return -1;
			}
		}
		
		if (i == 0) {
			*ptable = buf;
			fieldsNum++;
		}
		else {
			while(buf < bufEnd) {	
				if (*(buf + bufPos) == '\"') {	
					if(quoted == 0) {
						fprintf(stderr, "%s: drop line %lu, \" in unquoted field\n", argv0, lineno);
						return -1;
					}
					else if(quoted == 1 && bufPos < endQuotePos && bufPos > startBufPos) {
						fprintf(stderr, "%s: drop line %lu, quoted field not terminated\n", argv0, lineno);
						return -1;
					}
				}
				if ((*(buf + bufPos) == delim && quoted == 0) || (*( buf + bufPos) == delim && quoted == 1 && bufPos > endQuotePos)) {
					if(i == cnt){
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
					*(buf+bufPos) = '\0';
					bufPos++;
					lastIndex = bufPos - 1;
					i = cnt + 1;
					break;
				}
			bufPos++;
			}	
		}
	}
	
	if(*(buf+lastIndex) == delim || *(buf+lastIndex) == '\"') {
		fieldsNum++;
		
	}
	if (fieldsNum < cnt) {
		fprintf(stderr, "%s: drop line %lu, too few columns\n", argv0, lineno);
		return -1;
	}
	return 0;
}	



