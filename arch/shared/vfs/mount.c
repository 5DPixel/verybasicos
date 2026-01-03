#include <kernel/vfs/mount.h>
#include <kernel/mm/heap.h>

static struct mount_entry *mount_root;
static struct mount_entry *current_mount_entry;

void mount_init(){
	mount_root = (struct mount_entry *)kernel_heap_alloc(sizeof(struct mount_entry));
	current_mount_entry = mount_root;
}

struct mount_entry *mount_create_entry(const char *mount_path, const char *fs_name, const char *dev_name, struct superblock *superblock){
	struct mount_entry *entry = (struct mount_entry *)kernel_heap_alloc(sizeof(struct mount_entry));
	
	entry->mount_path = mount_path;
	entry->fs_name = fs_name;
	entry->dev_name = dev_name;
	entry->superblock = superblock;
	
	return entry;
}
