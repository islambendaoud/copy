
#define SUPER 0

static struct super_s super;

struct super_s{
	unsigned super_magic;
	unsigned super_serial;
	char super_name[32];
	unsigned super_root_inumber;
	unsigned super_first_free;
};

struct free_bloc_s{
	unsigned fb_nbbloc;
	unsigned fb_next;
}

void init_super(unsigned int vol){
	super.super_magic=SUPER_MAGIC;
	super.super_root_inumber=0;
	super.super_first_free=1;
	write_bloc_n(vol, SUPER, &super, sizeof(struct super_s));
	fb.fb_nbbloc=cylinder_of_bloc(vol, mbr.mbr_nsector;
	fb.fb_next=0;
	write_bloc_n(vol, 1, &fb, sizeof(struct free_bloc_s));
}

void load_super(unsigned int vol){
	read_bloc_n(vol,SUPER,&super,sizeof(struct super_s));
	assert(super.super_magic==SUPER_MAGIC);
}


unsigned int new_bloc(){
	int new;

	//cas ou il n'y a plus de bloc libre
	if(super.super_first_free==0)
		return 0;
	read_bloc_n(current_vol, super.super_first_free, &fb, sizeof(struct free_bloc_s));
	new = super.super_first_free;
	if(fb.fb_nbbloc==1)
		super.super_first_free = fb.fb_next;
	else {
		super.super_first_free++;
		fb.fb_nbbloc;
		wirte_bloc_s(current_vol, super.super_first_free, &fb, sizeof(struct free_bloc_s));
	}
	return new;
}


void free_bloc(unsigned int bloc){
	fb.fb_nbbloc = 1;
	fb.fb_next = super.super_first_free;
	super.super_first_free = bloc;
	write_bloc(current_vol, bloc, &fb, sizeof(struct free_bloc_s));
	

}
