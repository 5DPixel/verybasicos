#include <kernel/mm/heap.h>

void kernel_heap_init(int flags){
	struct rb_tree_allocator *alloc = (struct rb_tree_allocator *)initial_heap_data;
	
	enum rb_tree_allocator_flags alloc_flags;
	if(flags & KERNEL_PAGE_SIZE_4K){
		alloc_flags = RB_TREE_ALLOC_PAGE_SIZE_4K;
	}
	
	if(flags & KERNEL_PAGE_SIZE_2M){
		alloc_flags = RB_TREE_ALLOC_PAGE_SIZE_2M;
	}
	
	rb_tree_alloc_init(alloc, alloc_flags, (void *)initial_heap_data + sizeof(struct rb_tree_allocator));
}

void *kernel_heap_alloc_pages(int pages){
	struct rb_tree_allocator *alloc = (struct rb_tree_allocator *)initial_heap_data;
	
	return rb_tree_alloc_pages(alloc, pages, 0);
}

void kernel_heap_free_pages(int page_count, void *pages){
	struct rb_tree_allocator *alloc = (struct rb_tree_allocator *)initial_heap_data;
	
	rb_tree_free_pages(alloc, page_count, pages);
}
