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


int main(){
	int nbSec,nbCyl,cylinder,secteur,i;
	if(init_hardware("hardware.ini") == 0) {
		fprintf(stderr, "Error in hardware initialization\n");
		exit(EXIT_FAILURE);
    	}

	/* Interreupt handlers */
	for(i=0; i<16; i++)
		IRQVECTOR[i] = empty_it;

	/* Allows all IT */
	_mask(1);
	_out(HDA_CMDREG,CMD_DSKINFO);
	nbCyl=_in(HDA_DATAREGS)<<8;
	nbCyl+=_in(HDA_DATAREGS+1);
	nbSec=_in(HDA_DATAREGS+2)<<8;
	nbSec+=_in(HDA_DATAREGS+3);
	printf("Formatage du disque\n");
	for(cylinder=0;cylinder<nbCyl;cylinder++)
		for(secteur=0;secteur<nbSec;secteur++)
			format_sector(cylinder,secteur,1,0);
	return 0;
} 

