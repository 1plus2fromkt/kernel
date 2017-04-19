#ifndef AOUT_H
#define AOUT_H

#define MAGIC1 0407

struct exec {
	unsigned long   a_midmag;
	unsigned long   a_text;
	unsigned long   a_data;
	unsigned long   a_bss;
	unsigned long   a_syms;
	unsigned long   a_entry;
	unsigned long   a_trsize;
	unsigned long   a_drsize;
};

#endif 