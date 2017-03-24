#ifndef PAGING_H
#define PAGING_H

#define PAGE_NUMBER 10 // Number of pages, how much to give kernel for stack and etc. Oh, no, it is DIR_NUMBER. Should rename it
					   // Because 1 dir is 4 Mb, and 1 page is 4 Kb, but we need smth about Mbs

extern uint32_t pd[1024] __attribute__((aligned(4096)));
extern uint32_t pt[1024][1024] __attribute__((aligned(4096)));

#endif