/*
Author:
	DOUAILLE Erwan
	MIRANDA Yoan
*/

#include "drive.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

static void get_sector(unsigned int cylinder,unsigned int secteur){      
	_out(HDA_DATAREGS,(cylinder >>8) & 0xFF); 
	_out(HDA_DATAREGS+1,cylinder & 0xFF); 
	_out(HDA_DATAREGS+2,(secteur >>8) & 0xFF); 
	_out(HDA_DATAREGS+3,secteur & 0xFF); 
	_out(HDA_CMDREG,CMD_SEEK); 
	_sleep(HDA_IRQ); 
}

void read_sector(unsigned int cylinder,unsigned int secteur,unsigned char* buffer){
	int sectorsize;
	get_sector(cylinder,secteur);
	_out(HDA_DATAREGS,0);
	_out(HDA_DATAREGS+1,1);
	_out(HDA_CMDREG,CMD_READ);
	_sleep(HDA_IRQ);
	_out(HDA_CMDREG,CMD_DSKINFO);
	sectorsize=_in(HDA_DATAREGS+4)<<8;
	sectorsize+=_in(HDA_DATAREGS+5);
	memcpy(buffer,MASTERBUFFER,sectorsize);		
}

void read_sector_n(unsigned int cylinder, unsigned int secteur, unsigned char *buffer, int size){
	get_sector(cylinder,secteur);
	_out(HDA_DATAREGS,0);
	_out(HDA_DATAREGS+1,1);
	_out(HDA_CMDREG,CMD_READ);
	_sleep(HDA_IRQ);
	memcpy(buffer,MASTERBUFFER,size);	
}

void write_sector(unsigned int cylinder,unsigned int secteur,unsigned char* buffer){
	int sectorsize;	
	get_sector(cylinder,secteur);
	_out(HDA_DATAREGS,0);
	_out(HDA_DATAREGS+1,1);
	_out(HDA_CMDREG,CMD_WRITE);
	_sleep(HDA_IRQ);
	_out(HDA_CMDREG,CMD_DSKINFO);
	sectorsize=_in(HDA_DATAREGS+4)<<8;
	sectorsize+=_in(HDA_DATAREGS+5);
	memcpy(MASTERBUFFER,buffer,sectorsize);
}

void write_sector_n(unsigned int cylinder,unsigned int secteur,unsigned char* buffer,int size){
	get_sector(cylinder,secteur);
	_out(HDA_DATAREGS,0);
	_out(HDA_DATAREGS+1,1);
	_out(HDA_CMDREG,CMD_WRITE);
	_sleep(HDA_IRQ);
	memcpy(MASTERBUFFER,buffer,size);
}

void format_sector(unsigned int cylinder,unsigned int secteur,unsigned int nsecteur,unsigned int value){
	get_sector(cylinder,secteur);
	_out(HDA_DATAREGS,(nsecteur>>8) & 0xFF);
	_out(HDA_DATAREGS+1,nsecteur & 0xFF);
	_out(HDA_DATAREGS+2,(value>>24) & 0xFF);
	_out(HDA_DATAREGS+3,(value>>16) & 0xFF);
	_out(HDA_DATAREGS+4,(value>>8) & 0xFF);
	_out(HDA_DATAREGS+5,value & 0xFF);
	_out(HDA_CMDREG,CMD_FORMAT);
	_sleep(HDA_IRQ);
}

void
dump(unsigned char *buffer,
     unsigned int buffer_size,
     int ascii_dump,
     int octal_dump) 
{
    int i,j;
    
    for (i=0; i<buffer_size; i+=16) {
	/* offset */
	printf("%.8o",i);

	/* octal dump */
	if (octal_dump) {
	    for(j=0; j<8; j++)
		printf(" %.2x", buffer[i+j]);
	    printf(" - ");
	    
	    for( ; j<16; j++)
		printf(" %.2x", buffer[i+j]);
	    
	    printf("\n");
	}
	/* ascii dump */
	if (ascii_dump) {
	    printf("%8c", ' ');
	    
	    for(j=0; j<8; j++)
		printf(" %1c ", isprint(buffer[i+j])?buffer[i+j]:' ');
	    printf(" - ");
	    
	    for( ; j<16; j++)
		printf(" %1c ", isprint(buffer[i+j])?buffer[i+j]:' ');
	    
	    printf("\n");
	}
	
    }
}

