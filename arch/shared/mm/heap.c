#include <kernel/mm/heap.h>
#include <kernel/mm/slab.h>
#include <common.h>

static char *initial_heap_data;
static struct slab_alloc_pool initial_pool = {0};

void kernel_heap_init(int flags){
	initial_heap_data = (char *)_kernel_heap_start;
	
	struct rb_tree_allocator *alloc = (struct rb_tree_allocator *)initial_heap_data;
	struct slab_allocator *slab_alloc = (struct slab_allocator *)((char *)initial_heap_data + sizeof(struct rb_tree_allocator));
	
	memset(alloc, 0, sizeof(struct rb_tree_allocator));
	memset(slab_alloc, 0, sizeof(struct slab_allocator));
	
	enum rb_tree_allocator_flags alloc_flags;
	enum slab_alloc_pool_flags pool_alloc_flags;
	
	if(flags & KERNEL_PAGE_SIZE_4K){
		alloc_flags = RB_TREE_ALLOC_PAGE_SIZE_4K;
		pool_alloc_flags = SLAB_ALLOC_POOL_PAGE_SIZE_4K;
	}
	
	if(flags & KERNEL_PAGE_SIZE_2M){
		alloc_flags = RB_TREE_ALLOC_PAGE_SIZE_2M;
		pool_alloc_flags = SLAB_ALLOC_POOL_PAGE_SIZE_2M;
	}
	
	rb_tree_alloc_init(alloc, alloc_flags, (void *)((char *)(initial_heap_data + sizeof(struct rb_tree_allocator) + sizeof(struct slab_allocator))));
	
	slab_alloc_create_pool(slab_alloc, pool_alloc_flags, &initial_pool, 8);
}

void *kernel_heap_alloc_pages(int pages){
	struct rb_tree_allocator *alloc = (struct rb_tree_allocator *)initial_heap_data;
	
	return rb_tree_alloc_pages(alloc, pages, 0);
}

void kernel_heap_free_pages(int page_count, void *pages){
	struct rb_tree_allocator *alloc = (struct rb_tree_allocator *)initial_heap_data;
	
	rb_tree_free_pages(alloc, page_count, pages);
}

void *kernel_heap_alloc(size_t size){
	//struct slab_allocator *slab_alloc = (struct slab_allocator *)((char *)initial_heap_data + sizeof(struct rb_tree_allocator));
	
	return slab_alloc_pool_find_free(&initial_pool, size);
}

void kernel_heap_free(void *ptr, size_t size){
	struct slab_allocator *slab_alloc = (struct slab_allocator *)((char *)initial_heap_data + sizeof(struct rb_tree_allocator));
	
	slab_alloc_free(slab_alloc, ptr, size);
}
