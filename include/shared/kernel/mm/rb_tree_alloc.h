#ifndef KERNEL_SHARED_MM_RB_TREE_ALLOC_H
#define KERNEL_SHARED_MM_RB_TREE_ALLOC_H

#include <stdint.h>

enum rb_tree_allocator_flags {
	RB_TREE_ALLOC_PAGE_SIZE_4K = 0x00,
	RB_TREE_ALLOC_PAGE_SIZE_2M = 0x01 /* possibly for later, and maybe 1G too, also varied paging would be nice */
};

struct rb_tree_allocator {
	struct rb_tree *tree;
	struct rb_tree_node *current_parent;
	void *data;
	int current_page;
	int page_size;
};

void rb_tree_alloc_init(struct rb_tree_allocator *alloc, enum rb_tree_allocator_flags flags, void *data);
void *rb_tree_alloc_pages(struct rb_tree_allocator *alloc, int pages, uintptr_t page_start); /* page_start can be 0 */
void rb_tree_free_pages(struct rb_tree_allocator *alloc, int page_count, void *addr);

#endif
