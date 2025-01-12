/*
Author:
	DOUAILLE Erwan
	MIRANDA Yoan
*/

#include "hw_ini.h"
#include "hardware.h"
#include "drive.h"
#include <string.h>

#define MBR_MAGIC 0xA5E

enum voltype_e{VT_NO_VOL,VT_BASE,VT_AUX,VT_OTHER};

struct vol_descr_s{
	unsigned vol_cylinder;
	unsigned vol_sector;
	unsigned vol_nsector;
	enum voltype_e vol_voltype;
};


struct mbr_s{
	unsigned mbr_magic;
	unsigned mbr_nbvol;
	struct vol_descr_s mbr_vols[MAXVOL];
};

struct mbr_s mbr;

void save_mbr();
void load_mbr();
unsigned cylinder_of_bloc(int , int );
unsigned sector_of_bloc(int , int );
void read_bloc(unsigned int , unsigned int , unsigned char* );
void write_bloc(unsigned int , unsigned int , unsigned char* );
void read_bloc_n(unsigned int , unsigned int , unsigned char*,unsigned int );
void write_bloc_n(unsigned int , unsigned int , unsigned char*,unsigned int );
void format_vol(unsigned int );
char* printf_vol_type(unsigned int );
void printf_vol(unsigned int );
void list_volumes();
void init_mbr();
void new_volume(unsigned int, unsigned int, unsigned int, unsigned int);
void delete_volume(unsigned int );
