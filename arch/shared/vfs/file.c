#include <kernel/vfs/file.h>
#include <kernel/mm/heap.h>
#include <common.h>

static struct vfs_type fs_types = {0}; /* singly linked list */
static struct vfs_type *current_fs_type;

void init_vfs_types(){
	current_fs_type = &fs_types;
}

void register_vfs_type(struct vfs_type *type){
	current_fs_type->next = type;
	current_fs_type = type;
}

struct vfs_type *find_vfs_type(const char *name){
	while(current_fs_type->next != NULL){
		if(string_equals((char *)name, (char *)current_fs_type->name)){
			return current_fs_type;
		}
		
		if(current_fs_type->next == NULL) return NULL;
		
		current_fs_type = current_fs_type->next;
	}
	
	return NULL;
}
