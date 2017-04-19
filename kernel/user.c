#include "user.h"
#include "memory/v_allocator.h"
#include "memory/kmalloc.h"
#include "memory/memory.h"
#include "tty.h"
#include "paging.h"

uint32_t top_process = 0;

uint32_t *saved_pages[NUMBER_OF_PROCESSES];
uint32_t next_process[NUMBER_OF_PROCESSES]; // maybe it should be replaced with structure

#define SIZE(x) \
		(USER_PAGES * (sizeof(x)) + PAGE_SIZE) / PAGE_SIZE

int init_user_allocator()
{
	struct v_allocator* alloc = &process_allocators[top_process];
	struct mem_tree* tree = &process_trees[top_process];
	//init pointer for saving pages
	saved_pages[top_process] = (uint32_t*)kmalloc(((USER_PAGES + 1) * sizeof(uint32_t) + PAGE_SIZE - 1) / PAGE_SIZE, &kernel_allocator); 
	top_process = next_process[top_process];
	struct node* nodes = (struct node*)kmalloc(SIZE(struct node) * 2, &kernel_allocator);
	struct node** pnodes = (struct node**)kmalloc(SIZE(struct node*) * 2, &kernel_allocator);
	struct mem_entry* m_entry = (struct mem_entry*)kmalloc(SIZE(struct mem_entry), &kernel_allocator);
	m_entry[0].base = 0;
	m_entry[0].free = USER_PAGES;
	for (uint32_t i = 0; i < USER_PAGES; i++)
	{
		nodes[i].val = &m_entry[i];
		nodes[i].depth = 0;
		pnodes[i] = &nodes[i];
	}
	tree->nodes = pnodes;
	tree->top = 1;
	alloc->t = tree; 
	alloc->left_bound = 0;
	alloc->right_bound = kernel_allocator.left_bound - 1;
	for (uint32_t i = 0; i < KERNEL_START_INDEX; i++) // clearing pd and pt after another process. Need to save them when killing
	{
		pd[i] = 0;
		for (uint32_t j = 0; j < PAGE_TABLE_NUMBER; j++) 
		{
			pt[i][j] = 0;
		}
	} // should be replaced with memset, although I don't have one right now
	user_allocator = alloc;
	terminal_writestring("user allocator is initialized\n");
	return top_process - 1;
	// copypaste(( seen same code in kmalloc.c 
}

void save_pages(int pid)
{
	uint32_t cnt = 0;
	for (uint32_t i = 0; i < USER_DIRECTORIES; i++)
		saved_pages[pid][cnt++] = pd[i];
	for (uint32_t i = 0; i < USER_DIRECTORIES; i++)
		for (uint32_t j = 0; j < PAGE_TABLE_NUMBER; j++)
			saved_pages[pid][cnt++] = pt[i][j];
	// all of these should be replaced with memset
}

void restore_pages(int pid)
{
	uint32_t cnt = 0;
	for (uint32_t i = 0; i < USER_DIRECTORIES; i++)
		pd[i] = saved_pages[pid][cnt++];
	for (uint32_t i = 0; i < USER_DIRECTORIES; i++)
		for (uint32_t j = 0; j < PAGE_TABLE_NUMBER; j++)
			pt[i][j] = saved_pages[pid][cnt++];
}

void remove_process(int pid)
{
	next_process[pid] = top_process;
	top_process = pid;
}