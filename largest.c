#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "parking.h"

#ifndef MYLARGEST

#else

/*
 * largest
 *     find the vehicle with the largest number of
 *     tickets and largest fine in database
 *     and print it
 */
void
largest(void)
{
struct vehicle *count = NULL;   // vehicle with largest number of tickets
struct vehicle *fine = NULL;    // vehicle with largest total fine

unsigned int largestCount = 0;	//keeps track of current largest count
unsigned int largestFine = 0;	//keeps track of current largest fine

struct vehicle *vehicle;        //current vehicle being searched
for (uint32_t i = 0;i<tabsz;i++) {
	if (*(htable+i) != NULL) {
		vehicle = *(htable+i);
		while(vehicle != NULL) {
			if (vehicle->cnt_ticket >= largestCount) {
				largestCount = vehicle->cnt_ticket;
				count = vehicle;
			}
			if (vehicle->tot_fine >= largestFine) {
				largestFine = vehicle->tot_fine;
				fine = vehicle;
			}
			vehicle = vehicle->next;
		}
	}
}

if ((count == NULL) || (fine == NULL)) {
	printf("Empty database\n");
	return;
}
printf("Most tickets Plate: %s, State: %s tickets: %u, total fine: $%u\n",
	count->plate, count->state, count->cnt_ticket, count->tot_fine);
printf("Largest fine Plate: %s, State: %s tickets: %u, total fine: $%u\n",
	fine->plate, fine->state, fine->cnt_ticket, fine->tot_fine);
return;
}
#endif
