#include "v_allocator.h"

void put_entry(uint32_t base, uint32_t free, struct v_allocator* a)
{
	int ind = a->t->top;
	struct mem_entry* en = a->t->nodes[ind]->val;
	en->base = base;
	en->free = free;
	a->t->nodes[ind]->depth = 0;
	a->t->top++;
}

struct mem_entry* get_entry(uint32_t sz, uint32_t base, bool search_base, struct v_allocator* a)
{
	struct node** arr = a->t->nodes;
	for (uint32_t i = 0; i < a->t->top; i++)
	{
		if ((!search_base && arr[i]->val->free >= sz) || (search_base && arr[i]->val->free == sz && arr[i]->val->base == base))
			return arr[i]->val;
	}
	return 0; // panic
}

void remove_entry(uint32_t sz, struct v_allocator* a)
{
	struct node* x = a->t->nodes[--a->t->top];
	a->t->nodes[sz] = x;
}