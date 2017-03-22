#include "kmalloc.h"
#include "memory.h"
#include "kernel/paging.h"
#include <stdbool.h>
#include "kernel/kernel.h"
#include "kernel/tty.h"
#include "libc/itoa.h"
#include "v_allocator.h"

struct v_allocator user_allocator, kernel_allocator;
struct node user_nodes[USER_PAGES], kernel_nodes[KERNEL_PAGES], *puser_nodes[USER_PAGES], *pkernel_nodes[KERNEL_PAGES];
struct mem_entry user_entries[USER_PAGES], kernel_entries[KERNEL_PAGES];
struct mem_tree user_tree, kernel_tree;

// maybe user allocator is not needed here. Probably we don't need it

void init_mem_manager()
{
	user_entries[0].base = 0;
	user_entries[0].free = USER_PAGES;
	kernel_entries[0].base = KERNEL_START_INDEX * PAGE_TABLE_NUMBER;
	kernel_entries[0].free = KERNEL_PAGES;
	for (int i = 0; i < USER_PAGES; i++)
	{
		user_nodes[i].val = &user_entries[i];
		user_nodes[i].depth = 0;
		puser_nodes[i] = &user_nodes[i];
	}
	for (int i = 0; i < KERNEL_PAGES; i++)
	{
		kernel_nodes[i].val = &kernel_entries[i];
		kernel_nodes[i].depth = 0;
		pkernel_nodes[i] = &kernel_nodes[i];
	}
	user_tree.nodes = puser_nodes;
	user_tree.top = 1;
	kernel_tree.nodes = pkernel_nodes;
	kernel_tree.top = 1;
	kernel_allocator.t = &kernel_tree;
	user_allocator.t = &user_tree;
}

uint32_t* get_address(uint32_t page)
{
	return (uint32_t*)((page >> SYSTEM_BYTES) << SYSTEM_BYTES);
}

uint32_t* get_page(uint32_t ind)
{
	uint32_t dir_num = ind / PAGE_TABLE_NUMBER;
	uint32_t* page = get_address(pd[dir_num]);
	uint32_t offset = ind % PAGE_TABLE_NUMBER;
	return &page[offset]; 
}

// metainf -- the page number(!) of the end of the free block or the beginning
// Beginning of the free block holds the end meta, and vice versa(hope right spelling). Used for concatenation 
void set_meta_inf(uint32_t* page, uint32_t info) // put the beginning or the end of the free block to the free page
{
	(*page) = (*page & ((1 >> SYSTEM_BYTES) - 1)) | (info << SYSTEM_BYTES);
}

void* offsets_to_address(uint32_t pd_off, uint32_t pt_off, uint32_t off)
{
	return (void*) (((pd_off << PAGE_DIR_OFFSET)) + (pt_off << PAGE_TABLE_OFFSET) + off * PAGE_SIZE);
}

uint32_t address_to_dir_num(void* ad)
{
	return (uint32_t)ad >> PAGE_DIR_OFFSET;
}

uint32_t address_to_tab_num(void* ad)
{
	return ((uint32_t)ad >> PAGE_TABLE_OFFSET) & ((1 << PAGE_TABLE_BITS) - 1);
}

char buff[64];

void* kmalloc(uint32_t size, struct v_allocator* a)
{
	struct mem_entry* entry = get_entry(size, 0, false, a);
	uint32_t free = entry->free, base = entry->base;
	uint32_t end_of_free = base + free - 1;
	uint32_t* page;
	for (uint32_t i = base; i < base + size; i++)
	{
		page = get_page(i);
		uint32_t phys = (uint32_t)load_phys_page();
		(*page) = phys | PT_PRESENT | PT_RW;
	}
	if (size != free) 
	{
		page = get_page(base + size);
		set_meta_inf(page, end_of_free);
		put_entry(base + size, free - size, a);
	}
	uint32_t pd_offset = base / PAGE_TABLE_NUMBER;
	uint32_t pt_offset = base % PAGE_TABLE_NUMBER;
	return offsets_to_address(pd_offset, pt_offset, 0);
}

void concat(uint32_t page_num, uint32_t sz)
{
	// if (address ) 
	// here should be concatenation of free blocks. 
}

void kfree(void* address, uint32_t size, struct v_allocator* a)
{
	uint32_t dir_num = address_to_dir_num(address);
	uint32_t tab_num = address_to_tab_num(address);
	uint32_t page_num = dir_num * PAGE_TABLE_NUMBER + tab_num;
	put_entry(page_num, size, a);
	for (uint32_t i = page_num; i < size + page_num; i++)
	{
		uint32_t* page = get_page(i);
		free_phys_page(get_address(*page));
		*page = *page & (~PT_PRESENT);
	}
	set_meta_inf(get_page(page_num), page_num + size - 1);
	set_meta_inf(get_page(page_num + size - 1), page_num);
	concat(page_num, size);
}