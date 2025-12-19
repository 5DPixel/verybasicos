#include <kernel/mm/rb_tree_alloc.h>
#include <kernel/list/rb_tree.h>
#include <common.h>
#include <stddef.h>

void rb_tree_alloc_init(struct rb_tree_allocator *alloc, enum rb_tree_allocator_flags flags, void *data){
	if(alloc == NULL || data == NULL) return;

	int page_size = 0x1000; /* 4096 as a fallback */

	if(flags & PAGE_SIZE_4K)
		page_size = 0x1000;
	
	struct rb_tree *tree = (struct rb_tree *)data;
	memset(tree, 0, sizeof(struct rb_tree));

	data += sizeof(struct rb_tree);

	struct rb_tree_node *root = (struct rb_tree_node *)data;
	memset(root, 0, sizeof(struct rb_tree_node));

	data += sizeof(struct rb_tree_node);

	struct rb_tree_node *root_node = (struct rb_tree_node *)data;
	memset(root_node, 0, sizeof(struct rb_tree_node));

	root->key = ((uintptr_t)1 << 32) | CEIL_DIV((uintptr_t)data, page_size); /* page start */
	root->parent = NULL;
	
	root_node->colour = RB_TREE_NODE_BLACK;
	
	alloc->tree = tree;
	alloc->data = data;
	alloc->page_size = page_size;
}

void *rb_tree_alloc_pages(struct rb_tree_allocator *alloc, int pages, uintptr_t page_start){
	uintptr_t addr;
	addr = alloc->current_page * alloc->page_size;
	
	struct rb_tree_node *parent = (struct rb_tree_node *)alloc->data;
	
	alloc->data += sizeof(struct rb_tree_node);

	struct rb_tree_node *child = (struct rb_tree_node *)alloc->data;

	child->key = ((uintptr_t)pages << 32) | CEIL_DIV((uintptr_t)alloc->data, alloc->page_size); /* page start */
	child->parent = parent;

	if(page_start)
		addr = page_start * alloc->page_size;
	
	alloc->current_page++;
	
	return (void *)addr;
}
