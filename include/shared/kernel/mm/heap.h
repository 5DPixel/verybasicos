#ifndef KERNEL_SHARED_MM_HEAP_H
#define KERNEL_SHARED_MM_HEAP_H

#include <kernel/mm/rb_tree_alloc.h>

#define INITIAL_HEAP_DATA_SIZE 8192

static char initial_heap_data[INITIAL_HEAP_DATA_SIZE];

enum kernel_allocator_flags {
	KERNEL_PAGE_SIZE_4K = 0x00,
	KERNEL_PAGE_SIZE_2M = 0x01
};

void *kernel_heap_alloc_pages(int pages);
void kernel_heap_free_pages(int page_count, void *pages);
void kernel_heap_init(int flags);

#endif
