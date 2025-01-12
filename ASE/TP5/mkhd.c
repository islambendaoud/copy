/*
Author:
	DOUAILLE Erwan
	MIRANDA Yoan
*/

#include <stdlib.h>
#include <stdio.h>
#include "hardware.h"
#include "drive.h"

static void
empty_it()
{
    return;
}


int
main(int argc, char **argv)
{
    unsigned int i;
    
    /* init hardware */
    if(init_hardware("hardware.ini") == 0) {
	fprintf(stderr, "Error in hardware initialization\n");
	exit(EXIT_FAILURE);
    }

    /* Interreupt handlers */
    for(i=0; i<16; i++)
	IRQVECTOR[i] = empty_it;
	
    /* Allows all IT */
    _mask(1);

    /* and exit! */
    exit(EXIT_SUCCESS);
}
