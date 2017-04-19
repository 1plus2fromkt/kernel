#ifndef USER_H
#define USER_H

#include "memory/memory.h"

#define NUMBER_OF_PROCESSES 10
#define MY_STACK_SIZE 2 * PAGE_TABLE_NUMBER // stack size in number of pages

int init_user_allocator();
void save_pages(int pid);
void restore_pages(int pid);
void remove_process(int pid);

#endif