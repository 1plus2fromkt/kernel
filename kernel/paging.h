#ifndef PAGING_H
#define PAGING_H

#define DIR_NUMBER 10 // Number of directories, how much to give kernel for stack and etc.

extern uint32_t pd[1024] __attribute__((aligned(4096)));
extern uint32_t pt[1024][1024] __attribute__((aligned(4096)));

#endif