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

/*
 *  vehiclelookup
 *          look for vehicle in the database
 *          vehicle must match both plate and state strings
 *  args
 *  plate   plate id string to be found
 *  state   state id string to be found
 *
 *  returns  pointer to vehicle if found NULL otherwise
 */

struct vehicle *
vehiclelookup(char *plate, char *state)
{
	//struct vehicle head hashed from plate
	struct vehicle *Chain;	
	//value used to find Chain
	uint32_t hashval;	

	hashval = hash(plate) % tabsz;
	Chain = *(htable + hashval); // get list head pointer by plate
	while (Chain != NULL) {
		if(strcmp(plate, Chain->plate) == 0 && strcmp(state,Chain->state) == 0) {
			return Chain;
		}
	        else {
			Chain = Chain->next;
		}
	}
	return NULL;
}
#endif
