#ifndef KERNEL_SHARED_MM_SLAB_H
#define KERNEL_SHARED_MM_SLAB_H

/* assume heap was initialised */

#include <stdint.h>
#include <stddef.h>

#define SLAB_ALLOC_ORDER_COUNT 8
#define SLAB_ALLOC_ORDER_BASE 16

struct slab_allocator {
	struct page_slab *page_orders[SLAB_ALLOC_ORDER_COUNT];
};

struct page_slab {
	size_t page_count;
	size_t page_size;
	size_t split_size;
	size_t alloc_align;
	int free_item_count;
	uint8_t item_bitmap[];
};

static inline int page_slab_order(size_t size){
	if(size < SLAB_ALLOC_ORDER_BASE){
		return 0;
	}
	
	int index = __builtin_ctz(size) - __builtin_ctz(SLAB_ALLOC_ORDER_BASE); /* trailing zeros */

	return index;
}

void slab_alloc_init(struct slab_allocator *alloc);
void *slab_alloc_find_free(struct slab_allocator *alloc, size_t size);
void slab_alloc_free(struct slab_allocator *alloc, void *ptr, size_t size);

#endif
