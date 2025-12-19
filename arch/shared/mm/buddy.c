#include <kernel/mm/buddy.h>
#include <common.h>

static struct mm_buddy_entry *buddy_split(struct mm_buddy_entry *entry, size_t size){
	size_t split_size;

	if(!entry || size == 0) return NULL;
	
	while(size < entry->size){
		split_size = entry->size >> 1; /* /2 */
		entry->size = split_size;
		entry = buddy_next(entry);
		entry->size = split_size;
		entry->free = 1;
	}

	if(size <= entry->size) return entry;

	return NULL;
}

static inline size_t mm_buddy_required_size(struct mm_buddy_allocator *alloc, size_t size){
	size_t block_size = alloc->align;
	
	block_size += sizeof(struct mm_buddy_allocator);
	size = align_size_forward(size, alloc->align);
	
	while(size > block_size)
		block_size <<= 1; /* *2 */
	
	return block_size;
}

static struct mm_buddy_entry *buddy_find_best(struct mm_buddy_entry *head, struct mm_buddy_entry *tail, size_t size){
	struct mm_buddy_entry *chosen_block = NULL;
	struct mm_buddy_entry *block = head;
	struct mm_buddy_entry *buddy = buddy_next(block);
	
	if(block > tail || buddy > tail) return buddy_split(block, size);
	
	while(block < tail && buddy < tail){
		if(block->free && buddy->free && block->size == buddy->size){
			block->size <<= 1;
			if(size <= block->size)
				chosen_block = block;
			
			block = buddy_next(block);
			if(block < tail)
				buddy = buddy_next(block);
			
			continue;
		}
		
		if(size == block->size << 1)
			return buddy;
	}

	if(chosen_block != NULL)
		return chosen_block;
	
	return NULL;
}

void mm_buddy_init(struct mm_buddy_allocator *alloc, void *head, size_t size, size_t align){
	if(head == NULL) return;
	
	if(align < sizeof(struct mm_buddy_entry))
		align = sizeof(struct mm_buddy_entry);

	alloc->align = align;

	alloc->head = (struct mm_buddy_entry *)head;
	alloc->head->size = size;
	alloc->head->free = true;

	alloc->tail = buddy_next(head);
}
