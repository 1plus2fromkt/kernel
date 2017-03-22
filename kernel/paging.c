#include <stdint.h>
#include "paging.h"
#include "kernel.h"

uint32_t pd[1024] __attribute__((aligned(4096)));
uint32_t pt[1024][1024] __attribute__((aligned(4096)));
uint32_t pn = PAGE_NUMBER;

