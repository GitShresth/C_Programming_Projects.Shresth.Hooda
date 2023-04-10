#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "parking.h"
#include "hashdb.h"

#ifndef MYINSTICKET

#else
/*
 * insert ticket
 *      add a ticket (summons) to the database
 *
 *  plate   plate id string to be added
 *  state   state id string to be added
 *          summons code integer value used an an index into the fines 
 *   	    table
 *
 *  returns 0 if ok and -1 for all errors
 */

int
insertticket(char *summ, char *plate, char *state, char *date, int code)
{
    	unsigned long long summid;	/*summons number value*/
    	time_t dateval;			/*date value*/
	struct vehicle *Chain;		/*current vehicle*/
	uint32_t hashval;		/*location of chain*/
	hashval = hash(plate) % tabsz;
	Chain = *(htable + hashval);
	
	int fine = fineTab[code].fine;	/*value of fine*/
	int match = 0;			/*checks if vehicle exists*/
	
	if (strtosumid(summ, &summid) != 0) {
        	return -1;
	}
    	if (strtoDate(date, &dateval) != 0) {
        	return -1;
	}
	while (Chain != NULL) {
		if (strcmp(plate, Chain->plate) == 0 && strcmp(state, Chain->state) == 0) {
			struct ticket *tick = Chain->head;
			struct ticket *nextTick;
			struct ticket *newTick;

			newTick = (struct ticket*)malloc(sizeof(struct ticket));
			newTick->summons = summid;	
			newTick->date = dateval;
			newTick->code = code;
			newTick->next = NULL;

			if (tick != NULL) {
				nextTick = tick->next;
				while (nextTick != NULL) {
					if (tick->summons == summid) {
						fprintf(stderr,"%s: duplicate summons %llu\n", argv0, summid);
						return -1;
					}		
					tick = nextTick;
					nextTick = tick->next;
				}

				if (tick->summons == summid) {
					fprintf(stderr,"%s: duplicate summons %llu\n", argv0, summid);
					return -1;
				}
				tick->next = newTick;
			}
			else {
				if(tick->summons == summid) {
					fprintf(stderr,"%s: duplicate summons %llu\n", argv0, summid);
					return -1;
				}
				Chain->head = newTick;
			}
			Chain->cnt_ticket++;
			Chain->tot_fine += fine;
			match = 1;
			break;
		}	
		else {
			Chain = Chain->next;
		}
	}
	if (match == 0) {
		struct vehicle *newV;	/*new allocated vehicle*/
		newV = (struct vehicle*)malloc(sizeof(struct vehicle));
		newV->state = strdup(state);
		newV->plate = strdup(plate);

		struct vehicle *basevehicle = *(htable+hashval);	/*chain head*/
		struct ticket *newT;	/*new ticket*/

		newT = (struct ticket*)malloc(sizeof(struct ticket));
		newT->summons = summid;
		newT->date = dateval;
		newT->code = code;
		newT->next = NULL;
		newV->cnt_ticket = 1;
		newV->tot_fine = fine;
		newV->next = basevehicle;
		newV->head = newT; 
		*(htable+hashval) = newV;
	}
    return 0;
}
#endif
