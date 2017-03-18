#include <stdint.h>
#include "paging.h"

uint32_t pd[1024] __attribute__((aligned(4096)));
uint32_t pt[PAGE_NUMBER][1024] __attribute__((aligned(4096)));
uint32_t pn = PAGE_NUMBER;

// uint32_t pt1[1024] __attribute__((aligned(4096)));
// uint32_t pt2[1024] __attribute__((aligned(4096)));

