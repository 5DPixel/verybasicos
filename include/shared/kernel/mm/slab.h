#ifndef KERNEL_SHARED_MM_SLAB_H
#define KERNEL_SHARED_MM_SLAB_H

/* assume heap was initialised */

#include <stdint.h>
#include <stddef.h>

#define SLAB_ALLOC_ORDER_COUNT 8
#define SLAB_ALLOC_ORDER_BASE 16

enum slab_alloc_pool_flags {
	SLAB_ALLOC_POOL_PAGE_SIZE_4K = 0x01,
	SLAB_ALLOC_POOL_PAGE_SIZE_2M = 0x02,
};

struct slab_alloc_pool {
	struct page_slab *page_orders[SLAB_ALLOC_ORDER_COUNT];
	uint8_t *page_bitmaps[SLAB_ALLOC_ORDER_COUNT];
};

struct slab_allocator {
	int alloc_pool_index;
	struct slab_alloc_pool *alloc_pools[];
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
	
	return __builtin_ctz(size) - __builtin_ctz(SLAB_ALLOC_ORDER_BASE); /* trailing zeros */
}

static inline int page_free_items(struct page_slab *slab){
	return (slab->page_size / slab->split_size) * slab->page_count;
}

void slab_alloc_create_pool(struct slab_allocator *alloc, enum slab_alloc_pool_flags flags, struct slab_alloc_pool *pool, int page_count);
void slab_alloc_init(struct slab_allocator *alloc);
void *slab_alloc_pool_find_free(struct slab_alloc_pool *pool, size_t size);
void slab_alloc_free(struct slab_allocator *alloc, void *ptr, size_t size);

#endif
