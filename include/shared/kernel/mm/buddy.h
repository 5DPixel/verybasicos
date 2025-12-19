#ifndef KERNEL_MM_BUDDY_H
#define KERNEL_MM_BUDDY_H

#include <stddef.h>
#include <stdbool.h>

struct mm_buddy_entry {
	size_t size;
	bool free;
};

struct mm_buddy_allocator {
	struct mm_buddy_entry *tail;
	struct mm_buddy_entry *head;
	size_t align;
};

static inline struct mm_buddy_entry *buddy_next(struct mm_buddy_entry *entry){
	return (struct mm_buddy_entry *)((char *)entry + entry->size);
}

static inline size_t mm_buddy_required_size(struct mm_buddy_allocator *alloc, size_t size);
static struct mm_buddy_entry *buddy_split(struct mm_buddy_entry *entry, size_t size);
static struct mm_buddy_entry *buddy_find_best(struct mm_buddy_entry *head, struct mm_buddy_entry *tail, size_t size);
void mm_buddy_init(struct mm_buddy_allocator *alloc, void *head, size_t size, size_t align);

#endif
