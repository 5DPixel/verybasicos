#ifndef SHARED_LIST_H
#define SHARED_LIST_H

#include <stddef.h>

struct circular_buffer {
	void *buffer;
	void *buffer_end;
	size_t capacity;
	size_t count;
	void *head;
	void *tail;
};

void cb_init(struct circular_buffer *cb, size_t capacity, size_t size);
void cb_free(struct circular_buffer *cb);
void cb_push_back(struct circular_buffer *cb, const void *item);
const void *cb_pop_front(struct circular_buffer *cb); 

#endif
