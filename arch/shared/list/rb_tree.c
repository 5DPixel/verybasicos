#include <kernel/list/rb_tree.h>

struct rb_tree_node *rb_tree_rotate_subtree(struct rb_tree *tree, struct rb_tree_node *sub, enum rb_tree_node_direction direction){
    struct rb_tree_node *sub_parent = sub->parent;
    struct rb_tree_node *root =  sub_parent->children[1 - direction]; /* sibling of subtree */
    struct rb_tree_node *child = root->children[direction];

    if(child)
        child->parent = sub;

    root->children[direction] = sub;
    sub->parent = root;

    if(sub_parent){
        sub_parent->children[sub == sub_parent->right] = root; /* if direction is right, child is right, etc */
    } else {
        tree->root = root; /* no subtree, this must be the first tree */
    }

    return root;
}

void rb_tree_insert(struct rb_tree *tree, struct rb_tree_node *node, struct rb_tree_node *parent){
    node->colour = RB_TREE_NODE_RED;

    struct rb_tree_node *grandparent;
    struct rb_tree_node *uncle;
    enum rb_tree_node_direction direction;

    if(!parent){
        tree->root = node;
        return;
    }

    while((node = node->parent)){
        if(node->parent->colour == RB_TREE_NODE_BLACK){
            return; /* no reordering needed, parent is black and child is red, which is fine */
        }

        grandparent = node->parent->parent;
        direction = rb_tree_node_direction(parent);
		uncle = grandparent->children[1 - direction];


        if(!uncle || uncle->colour == RB_TREE_NODE_BLACK){
            if(node == parent->children[1 - direction]){
                rb_tree_rotate_subtree(tree, parent, direction);
                node = node->parent;
                parent = grandparent->children[direction];
            }

            rb_tree_rotate_subtree(tree, grandparent, 1 - direction);
            parent->colour = RB_TREE_NODE_BLACK;
            grandparent->colour = RB_TREE_NODE_RED;
        }

        uncle->colour = RB_TREE_NODE_BLACK;
        parent->colour = RB_TREE_NODE_BLACK;

        grandparent->colour = RB_TREE_NODE_RED;
        node = grandparent;
    }

    return;
}

void rb_tree_delete(struct rb_tree *tree, struct rb_tree_node *node){
	struct rb_tree_node *parent = node->parent;
	struct rb_tree_node *sibling;
	struct rb_tree_node *close_nephew;
	struct rb_tree_node *far_nephew;
	
	enum rb_tree_node_direction direction = rb_tree_node_direction(node);
	parent->children[direction] = NULL;
	
	goto balance;
	
	while((node = node->parent)){
		direction = rb_tree_node_direction(node);
	balance:
		if(!parent){
			return;
		}
			
		sibling = parent->children[1 - direction];
		far_nephew = sibling->children[1 - direction];
		close_nephew = sibling->children[direction];
		
		if(sibling->colour == RB_TREE_NODE_RED){
			rb_tree_rotate_subtree(tree, parent, direction);
			parent->colour = RB_TREE_NODE_RED;
			sibling->colour = RB_TREE_NODE_BLACK;
			sibling = close_nephew;
			
			far_nephew = sibling->children[1 - direction];
			goto delete_case_6;
		}
		
		if(far_nephew && far_nephew->colour == RB_TREE_NODE_RED){
			goto delete_case_6;
		}
		
		if(close_nephew && close_nephew->colour == RB_TREE_NODE_RED){
			goto delete_case_5;
		}
		
		if(parent->colour == RB_TREE_NODE_RED){
			sibling->colour = RB_TREE_NODE_RED;
			parent->colour = RB_TREE_NODE_BLACK;
			return;
		}
		
		sibling->colour = RB_TREE_NODE_RED;
		node = parent;
	}
	
delete_case_5:
	rb_tree_rotate_subtree(tree, sibling, 1 - direction);
	sibling->colour = RB_TREE_NODE_RED;
	close_nephew->colour = RB_TREE_NODE_BLACK;
	far_nephew = sibling;
	sibling = close_nephew;

delete_case_6:
	rb_tree_rotate_subtree(tree, parent, direction);
	sibling->colour = RB_TREE_NODE_RED;
	parent->colour = RB_TREE_NODE_BLACK;
	far_nephew->colour = RB_TREE_NODE_BLACK;
	return;
}

struct rb_tree_node *rb_tree_search(struct rb_tree_node *node, uintptr_t key){
	if(node->key < node->right->key){
		rb_tree_search(node->right, key);
	}
	
	if(node->key > node->left->key){
		rb_tree_search(node->left, key);
	}
	
	return node;
}

struct rb_tree_node *rb_tree_next_free(struct rb_tree_node *node){
	if(node->left != NULL){
		return rb_tree_next_free(node->left);
	}
	
	if(node->right != NULL){
		return rb_tree_next_free(node->right);
	}
	
	return node;
}

uintptr_t rb_tree_closest_key(struct rb_tree_node *node){
	if(node->left && node->left->key == 0){
		return rb_tree_closest_key(node->left);
	}
	
	if(node->right && node->right->key == 0){
		return rb_tree_closest_key(node->right);
	}

	if(node->parent){
		return node->parent->key;
	}
	
	return node->key;
}
