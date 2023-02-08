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

#ifndef MYDELTICKET

#else

/*
 * delticket
 *      remove ticket (summons) from the database
 *      
 *
 * Inputs:
 *  plate   plate id string to be found
 *  state   state id string to be found
 *  summ    summon id string to be found
 *
 * returns 0 if ok -1 if not found or error
 */
int
delticket(char *plate, char *state, char *summ)
{
	unsigned long long summid;	/*summons value*/
	struct vehicle *Chain;		/*vehicle pointer*/
	uint32_t hashval;		/*points to veh chain*/
	hashval = hash(plate) % tabsz;	
	Chain = *(htable+hashval);
	
	if (strtosumid(summ, &summid) != 0)
        	return -1;

	struct vehicle *prevV = NULL;
	while (Chain != NULL) {
		if (strcmp(plate, Chain->plate) == 0 && strcmp(state, 
		   Chain->state) == 0) {
			break;
		}
		prevV = Chain;
		Chain = Chain->next;
	}

	if (Chain == NULL)
		return -1;
	
	struct ticket *tickSearch = Chain->head;    /*current ticket*/
	struct ticket *prevTick = NULL;		    /*ticket before*/
	while (tickSearch != NULL) {
		if (tickSearch->summons == summid)
			break;
		prevTick = tickSearch;
		tickSearch = tickSearch->next;
	}
	if (tickSearch == NULL) {
		return -1;
	}
unsigned long long fine = fineTab[tickSearch->code].fine; /*fine val*/
	Chain->tot_fine -= fine;
	Chain->cnt_ticket--;
	if (prevTick != NULL) 
		prevTick->next = tickSearch->next;
	else
		Chain->head = tickSearch->next;
	free(tickSearch);
	
	if (Chain->cnt_ticket == 0) {
		if (prevV != NULL) 
			prevV->next = Chain->next;
		else 
			*(htable+hashval) = Chain->next;
		free(Chain->plate);
		free(Chain->state);
		free(Chain);
	}
    return 0;
}
#endif
