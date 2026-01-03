#include <kernel/vfs/dentry_cache.h>
#include <kernel/list/hash.h>

static struct string_hash_table *dentry_cache;
static const int MAX_DENTRY_COUNT = 10; /* initially for testing */

void dentry_cache_init(){
	dentry_cache = create_string_hash_table_basic(MAX_DENTRY_COUNT);
}

void dentry_cache_new(char *file_name, struct dentry *dentry){
	if(dentry_cache == NULL) return;
	
	string_hash_insert(dentry_cache, file_name, (uintptr_t)dentry);
}

struct dentry *dentry_cache_search(char *file_name){
	return (struct dentry *)string_hash_search(dentry_cache, file_name);
} 
