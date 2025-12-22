#include <kernel/mm/slab.h>
#include <kernel/mm/heap.h>
#include <common.h>

void slab_alloc_init(struct slab_allocator *alloc){
	int i;
	int slab_order = SLAB_ALLOC_ORDER_BASE;
	
	for(i = 0; i < SLAB_ALLOC_ORDER_COUNT; i++){
		struct page_slab *slab = (struct page_slab *)kernel_heap_alloc_pages(1);
		slab->page_count = 1;
		slab->page_size = 4096; /* TODO: dynamically get page size */
		slab->split_size = slab_order;
		slab->free_item_count = slab->page_size / slab->split_size;
		memset(slab->item_bitmap, 0, slab->page_size / slab->split_size);
		
		alloc->page_orders[i] = slab;
		
		slab_order <<= 1;
	}
}

void *slab_alloc_find_free(struct slab_allocator *alloc, size_t size){
	struct page_slab *slab = alloc->page_orders[page_slab_order(size)];
	int bit = 7;
	int byte = 0;
	int i;
	
	size = align_size_forward(size, SLAB_ALLOC_ORDER_BASE);
	
	if(slab->free_item_count == 0){
		int index = page_slab_order(size);
		struct page_slab *slab = (struct page_slab *)kernel_heap_alloc_pages(1);
		slab->page_size = 4096;
		slab->split_size = size;
		slab->free_item_count = slab->page_size / slab->split_size;
		memset(slab->item_bitmap, 0, slab->page_size / slab->split_size);
		
		alloc->page_orders[index] = slab;
		slab->item_bitmap[0] |= (1 << 7);
		return (void *)((char *)slab + sizeof(struct page_slab));
	}
	
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
	
	return (void *)((char *)slab + sizeof(struct page_slab) + slab->split_size * i);
}

void slab_alloc_free(struct slab_allocator *alloc, void *ptr, size_t size){
	struct page_slab *slab = alloc->page_orders[page_slab_order(size)];
	uintptr_t slab_index = (uintptr_t)ptr - (uintptr_t)slab / (uintptr_t)slab->split_size;
	
	slab->free_item_count++;
	int bit = slab_index / 8;
	int byte = slab_index % 8;
	
	slab->item_bitmap[byte] &= (1 << bit);
}
