#include <kernel/list/hash.h>
#include <kernel/mm/heap.h>
#include <common.h>

struct string_hash_table *create_string_hash_table_basic(int entry_count){
	struct string_hash_table *table = (struct string_hash_table *)kernel_heap_alloc(sizeof(struct string_hash_table));
	memset(table, 0, sizeof(struct string_hash_table));
	
	table->hash = string_hash_basic;
	table->probe_free = probe_free_index;
	table->entry_count = entry_count;
	
	table->entries = (struct string_hash_entry *)kernel_heap_alloc(sizeof(struct string_hash_entry) * entry_count);
	memset(table->entries, 0, sizeof(struct string_hash_entry) * entry_count);
	
	return table;
}

int string_hash_basic(struct string_hash_table *hash_table, int rehash, char *string){
	int i, j = 0;
	
	for(i = 0; i < string_len((char *)string); i++){
		j += (int)string[i];
	}
	
	j += rehash;
	j %= hash_table->entry_count;
	
	return j;
}

int probe_free_index(struct string_hash_table *hash_table, char *string){
	int hash = hash_table->hash(hash_table, 0, string);
	int i;
	
	for(i = 0; hash_table->entries[hash].key != NULL; i++){
		hash = hash_table->hash(hash_table, i, string);
		
		if(i == hash_table->entry_count){
			return hash;
		}
		
		//if(i > hash_table->entry_count) TODO: resize
			//string_hash_add_size(hash_table, i - hash_table->entry_count);
	}
	
	return hash;
}

void string_hash_insert(struct string_hash_table *hash_table, char *key, uintptr_t value){
	int hash = hash_table->probe_free(hash_table, key);
	
	hash_table->entries[hash].value = value;
}

uintptr_t string_hash_search(struct string_hash_table *hash_table, char *key){
	int hash = hash_table->hash(hash_table, 0, key);
	int i;
	
	for(i = 0; string_equals(hash_table->entries[hash].key, key) == -1 || hash_table->entries[hash].key == NULL; i++){
		hash = hash_table->hash(hash_table, i, key);
		
		if(i == hash_table->entry_count)
			return hash_table->entries[hash].value;
	}
	
	return hash_table->entries[hash].value;
}
