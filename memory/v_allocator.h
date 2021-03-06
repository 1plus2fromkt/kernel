#ifndef V_ALLOCATOR_H
#define V_ALLOCATOR_H

#include "stdint.h"
#include <stdbool.h>

struct mem_entry
{
	uint32_t free; // In number of pages length of a free block
	uint32_t base; // In number of pages start of a free block
};

struct mem_tree
{
	struct node
	{	
		struct mem_entry* val;
		uint32_t depth;
	} **nodes;
	uint32_t top;
};

struct v_allocator
{
	struct mem_tree* t;
	uint32_t left_bound, right_bound; // left and right bounds of pages of this allocator.
};

void remove_entry(uint32_t sz, struct v_allocator* a);

struct mem_entry* get_entry(uint32_t sz, uint32_t base, bool search_base, struct v_allocator* a);

void put_entry(uint32_t base, uint32_t size, struct v_allocator* a);

bool is_my_page(uint32_t page_num, struct v_allocator* a);

void show_entries(struct v_allocator* a);

#endif