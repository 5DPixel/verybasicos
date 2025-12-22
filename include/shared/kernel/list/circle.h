#ifndef KERNEL_SHARED_LIST_CIRCLE_H
#define KERNEL_SHARED_LIST_CIRCLE_H

#include <stdatomic.h>

struct lf_circular_buffer {
	_Atomic void *tail;
	_Atomic void *head;
	size_t item_size;
	size_t capacity;
	size_t item_count;
};

#endif
