#ifndef BOOT_SHARED_LOG_H
#define BOOT_SHARED_LOG_H

#include <stddef.h>
#include <boot/shared/platform.h>

struct platform_model;
struct text_attributes;

struct log_circular_buffer {
    void *buffer;
    void *buffer_end;
    size_t capacity;
    size_t count; /* current number of items */
    void *head;
    void *tail;
};

enum log_severity {
	LOG_SEVERITY_NONE = 0,
	LOG_SEVERITY_DEBUG = 1,
	LOG_SEVERITY_WARN = 2,
	LOG_SEVERITY_ERROR = 3
};

struct log_record {
    uint64_t timestamp;
    enum log_severity severity;
	uint32_t message_length;
	char message[];
};

void log_cb_init(struct platform_model *model, struct log_circular_buffer *cb, size_t capacity);
void log_cb_free(struct platform_model *model, struct log_circular_buffer *cb);
void log_cb_push_back(struct platform_model *model, struct log_circular_buffer *cb, struct log_record *record);
void log_cb_pop_front(struct platform_model *model, struct log_circular_buffer *cb, struct log_record *item);
struct log_record *create_log(struct platform_model *model, char *message, enum log_severity severity);
void log_cb_entry(struct platform_model *model, struct log_circular_buffer *cb, struct text_attributes *attr);

#endif
