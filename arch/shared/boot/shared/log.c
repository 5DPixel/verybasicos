#include <boot/shared/log.h>

void log_cb_init(struct platform_model *model, struct log_circular_buffer *cb, size_t capacity){
	cb->buffer = model->alloc(model, capacity * sizeof(struct log_record));

	cb->buffer_end = (char *)cb->buffer + capacity * sizeof(struct log_record);
    cb->capacity = capacity;
    cb->count = 0;
    cb->head = cb->buffer;
    cb->tail = cb->buffer;
}

void log_cb_free(struct platform_model *model, struct log_circular_buffer *cb){
	(void)model;
	(void)cb;
	int i;
	
	for(i = 0; i < (int)cb->capacity; i++)
		model->free(model, (void *)cb->tail);
	model->free(model, (void *)cb->buffer);
}

void log_cb_push_back(struct platform_model *model, struct log_circular_buffer *cb, struct log_record *record){
	(void)model;
	size_t size = record->message_length * sizeof(char) + sizeof(struct log_record);

	if(cb->count == cb->capacity) return;		
	
	memcpy(cb->head, record, size);
	
	cb->head = (char *)cb->head + size;
	if(cb->head == cb->buffer_end)
		cb->head = cb->buffer;
	cb->count++;
}

void log_cb_pop_front(struct platform_model *model, struct log_circular_buffer *cb, struct log_record *item){
	(void)model;

	if(item == NULL) return;
	struct log_record *record = (struct log_record *)cb->tail;
	size_t size = record->message_length * sizeof(char) + sizeof(struct log_record);

	memcpy(item, cb->tail, size);
	cb->tail = (char *)cb->tail + size;
	if(cb->tail == cb->buffer_end)
		cb->tail = cb->buffer;
	
	cb->count--;


}

struct log_record *create_log(struct platform_model *model, char *message, enum log_severity severity){
	struct log_record *log = model->alloc(model, sizeof(struct log_record));
	log->message_length = string_len(message) + 1;
	log->severity = severity;
	
	memcpy(log->message, message, (log->message_length + 1) * sizeof(char));

	return log;
}

void log_cb_entry(struct platform_model *model, struct log_circular_buffer *cb, struct text_attributes *attr){
	struct log_record *log = (struct log_record *)model->alloc(model, sizeof(struct log_record));
	log_cb_pop_front(model, cb, log);

	model->write(model, log->message, attr);
}
