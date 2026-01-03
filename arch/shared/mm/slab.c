#include <kernel/mm/slab.h>
#include <kernel/mm/heap.h>
#include <common.h>

void slab_alloc_create_pool(struct slab_allocator *alloc, enum slab_alloc_pool_flags flags, struct slab_alloc_pool *pool, int page_count){
	int i;
	int slab_order = SLAB_ALLOC_ORDER_BASE;
	
	int page_size = 0x1000;
	
	if(flags & SLAB_ALLOC_POOL_PAGE_SIZE_4K)
		page_size = 0x1000;
	
	alloc->alloc_pool_index = 0;
	
	for(i = 0; i < SLAB_ALLOC_ORDER_COUNT; i++){
		struct page_slab *slab = (struct page_slab *)kernel_heap_alloc_pages(page_count);
		
		alloc->alloc_pools[alloc->alloc_pool_index] = pool;
		
		slab->page_count = page_count;
		slab->page_size = page_size;
		slab->split_size = slab_order;
		slab->free_item_count = page_free_items(slab);
		memset(slab->item_bitmap, 0, page_free_items(slab));
		
		pool->page_orders[i] = slab;
		
		slab_order <<= 1;
	}
}

//void slab_alloc_init(struct slab_allocator *alloc){
	//int i;
	//int slab_order = SLAB_ALLOC_ORDER_BASE;
	
	//for(i = 0; i < SLAB_ALLOC_ORDER_COUNT; i++){
		//struct page_slab *slab = (struct page_slab *)kernel_heap_alloc_pages(1);
		//memset(slab, 0, sizeof(struct page_slab));
		//struct page_slab *slab = (struct page_slab *)kernel_heap_alloc_pages(1);
		
		//uint8_t *page = (uint8_t *)kernel_heap_alloc_pages(1); /* TODO: this is really wasteful, but I guess not necessarily if a page of bits are used */
		
		//slab->page_count = 1;
		//slab->page_size = 4096; /* TODO: dynamically get page size */
		//slab->split_size = slab_order;
		//slab->free_item_count = page_free_items(slab);
		//slab->item_bitmap = (uint8_t *)((char *)slab + offsetof(struct page_slab, item_bitmap)); /* overwrites pages flexible array */
		//memset(slab->item_bitmap, 0, page_free_items(slab));
		
		//alloc->page_orders[i] = slab;
		
		//slab_order <<= 1;
	//}
//}

void *slab_alloc_pool_find_free(struct slab_alloc_pool *pool, size_t size){
	size = align_size_power_of_two(size);
	
	struct page_slab *slab = pool->page_orders[page_slab_order(size)]; /* TODO: check gdb for size and slab addr */
	int bit = 7;
	int byte = 0;
	int i;
	
	memset(slab->item_bitmap, 0, page_free_items(slab));
	
	if(slab->free_item_count == 0) return NULL; /* could allocate a new page, but since it's per alloc pool, signal that we're out of memory */
	
	/* TODO: bitset/BST */
	for(i = 0; i < ((int)(slab->page_size / slab->split_size) * 8); i++){
		if(!(slab->item_bitmap[byte] & (1 << bit))) break;
		
		bit--;
		if(bit < 0){
			bit = 7;
			byte++;
		}
	}
	
	slab->free_item_count--;	
	slab->item_bitmap[byte] |= (1 << bit);
	
	return (void *)((char *)slab + sizeof(struct page_slab) + page_free_items(slab) + slab->split_size * i);
}

//void slab_alloc_free(struct slab_allocator *alloc, void *ptr, size_t size){
	//struct page_slab *slab = alloc->page_orders[page_slab_order(size)];
	//uintptr_t slab_index = (uintptr_t)ptr - (uintptr_t)slab / (uintptr_t)slab->split_size;
	
	//slab->free_item_count++;
	//int bit = slab_index / 8;
	//int byte = slab_index % 8;
	
	//slab->item_bitmap[byte] &= (1 << bit);
//}
