#ifndef KERNEL_SHARED_LIST_RB_TREE_H
#define KERNEL_SHARED_LIST_RB_TREE_H

#include <stdint.h>
#include <stddef.h>

/* rb tree header definitions */
/* rb trees guarantee a worst case of space-time complexity O(log(n)) for searching, O(1) for rotating (as it's just re-ordering pointers), and a space complexity of O(n), but more realistically that would be the size of the struct accounting for alignment */

enum rb_tree_node_colour {
	RB_TREE_NODE_BLACK = 0,
	RB_TREE_NODE_RED = 1
};

enum rb_tree_node_direction {
	RB_TREE_NODE_LEFT = 0,
	RB_TREE_NODE_RIGHT = 1
};

struct rb_tree_node {
	struct rb_tree_node *parent;
	union {
		struct {
			struct rb_tree_node *left;
			struct rb_tree_node *right;
		};
		
		struct rb_tree_node *children[2];
	};
	enum rb_tree_node_colour colour;
	uintptr_t key; /* lower 32 bits -- start page, upper -- flags */
};

struct rb_tree {
	struct rb_tree_node *root;
};

static inline enum rb_tree_node_direction rb_tree_node_direction(struct rb_tree_node *node){
	return node == node->parent->right ? RB_TREE_NODE_RIGHT : RB_TREE_NODE_LEFT;
}

struct rb_tree_node *rb_tree_rotate_subtree(struct rb_tree *tree, struct rb_tree_node *sub, enum rb_tree_node_direction direction);
void rb_tree_insert(struct rb_tree *tree, struct rb_tree_node *node, struct rb_tree_node *parent);
void rb_tree_delete(struct rb_tree *tree, struct rb_tree_node *node);
struct rb_tree_node *rb_tree_search(struct rb_tree_node *node, uintptr_t key);
struct rb_tree_node *rb_tree_next_free(struct rb_tree_node *node);
uintptr_t rb_tree_closest_key(struct rb_tree_node *node);

#endif
