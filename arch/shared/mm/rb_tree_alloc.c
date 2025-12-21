#include <kernel/mm/rb_tree_alloc.h>
#include <kernel/list/rb_tree.h>
#include <common.h>
#include <stddef.h>

void rb_tree_alloc_init(struct rb_tree_allocator *alloc, enum rb_tree_allocator_flags flags, void *data){
	if(alloc == NULL || data == NULL) return;

	int page_size = 0x1000; /* 4096 as a fallback */

	if(flags & RB_TREE_ALLOC_PAGE_SIZE_4K)
		page_size = 0x1000;
	
	alloc->data = data;
	
	struct rb_tree *tree = (struct rb_tree *)data;
	memset(tree, 0, sizeof(struct rb_tree));

	struct rb_tree_node *root = (struct rb_tree_node *)((char *)data + sizeof(struct rb_tree));
	memset(root, 0, sizeof(struct rb_tree_node));

	root->key = ((uintptr_t)1 << 32) | CEIL_DIV((uintptr_t)data, page_size); /* page start */
	root->colour = RB_TREE_NODE_BLACK;
	
	tree->root = root;
	
	alloc->tree = tree;
	alloc->page_size = page_size;
	alloc->current_parent = root;
}

void *rb_tree_alloc_pages(struct rb_tree_allocator *alloc, int pages, uintptr_t page_start){
	uintptr_t addr;
	
	struct rb_tree_node *parent = alloc->current_parent;
	struct rb_tree_node *child = (struct rb_tree_node *)((char *)parent + sizeof(struct rb_tree_node));
	memset(child, 0, sizeof(struct rb_tree_node));
	
	uintptr_t node_page_count = parent->key & 0xFFFFFFFF;
	uintptr_t node_page_start = parent->key >> 32; /* TODO: change 1 to previous page count */

	child->key = ((uintptr_t)pages << 32) | (node_page_start + node_page_count); /* page start */
	rb_tree_insert(alloc->tree, child, parent);
	
	addr = (node_page_start + node_page_count) * alloc->page_size;

	if(page_start != 0){
		addr = page_start * alloc->page_size;
		child->key = ((uintptr_t)pages << 32) | page_start;
	}
	
	alloc->current_parent = child;
	
	return (void *)addr;
}

void rb_tree_free_pages(struct rb_tree_allocator *alloc, int page_count, void *addr){
	uintptr_t key = ((uintptr_t)page_count << 32) | ((uintptr_t)addr / alloc->page_size);
	struct rb_tree_node *page_node = rb_tree_search(alloc->tree->root, key);
	
	if(page_node){
		rb_tree_delete(alloc->tree, page_node); 
	}
	
	alloc->current_parent = page_node;
}
