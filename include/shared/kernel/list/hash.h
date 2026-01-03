#ifndef KERNEL_SHARED_LIST_HASH_H
#define KERNEL_SHARED_LIST_HASH_H

#include <stdint.h>

struct string_hash_table {
	int (*hash)(struct string_hash_table *, int, char *);
	int (*probe_free)(struct string_hash_table *, char *);
	int entry_count;
	struct string_hash_entry *entries;
};

struct string_hash_entry {
	char *key;
	uintptr_t value;
};

struct string_hash_table *create_string_hash_table_basic(int entry_count);
int string_hash_basic(struct string_hash_table *hash_table, int rehash, char *string);
int probe_free_index(struct string_hash_table *hash_table, char *string);
void string_hash_insert(struct string_hash_table *hash_table, char *key, uintptr_t value);
void string_hash_add_size(struct string_hash_table *hash_table, uint32_t add);
uintptr_t string_hash_search(struct string_hash_table *hash_table, char *key);

#endif
