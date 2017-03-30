#ifndef PIC_H
#define PIC_H

#include <stdint.h>

void pic_send_eoi(uint8_t irq);
void pic_remap(int offset1, int offset2);

#endif