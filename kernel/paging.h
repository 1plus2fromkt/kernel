#ifndef PAGING_H
#define PAGING_H

#define PAGE_NUMBER 10

extern uint32_t pd[1024] __attribute__((aligned(4096)));
extern uint32_t pt[1024][1024] __attribute__((aligned(4096)));

#endif