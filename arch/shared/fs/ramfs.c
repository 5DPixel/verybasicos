#include <kernel/fs/ramfs.h>
#include <kernel/vfs/file.h>
#include <kernel/mm/heap.h>
#include <kernel/vfs/dentry_cache.h>
#include <common.h>

static struct vfs_type ramfs_vfs = {0};

static struct inode_file_ops file_ops = {0};
static struct inode_file_ops dir_ops = {0};

static struct inode_ops inode_ops = {0};

static struct vfs_ops ops = {0};

void ramfs_init(struct superblock *superblock){
	const char *ramfs_name = "ramfs";
	ramfs_vfs.name = ramfs_name;
	superblock->inode_count = 0;
	
	ops.mount = ramfs_mount;
	inode_ops.create = ramfs_create;
	
	file_ops.read = ramfs_read_file;
	file_ops.write = ramfs_write_file;
	
	register_vfs_type(&ramfs_vfs);
}

struct inode *ramfs_create_inode(struct superblock *superblock, enum inode_type type, struct vfs_mount *mount, void *data, size_t data_size, struct inode *last){
	struct inode *inode = (struct inode *)kernel_heap_alloc(sizeof(struct inode));
	memset(inode, 0, sizeof(struct inode));
	
	inode->type = type;
	inode->reference_count = 0;
	inode->pending_writes = 0;
	inode->mount = mount;
	inode->data = data;
	inode->data_size = data_size; /* may not need */
	inode->superblock = superblock;
	
	if(last != NULL) last->next = inode;
	
	switch(type){
		default: {
			inode->file_ops = &file_ops;
			break;
		}
		
		case INODE_DIRECTORY: {
			inode->file_ops = &dir_ops;
			break;
		}
	}
	
	superblock->inode_count++;
	
	return inode;
}	

struct dentry *ramfs_create_dentry(struct inode *inode, char *name){
	struct dentry *dentry = (struct dentry *)kernel_heap_alloc(sizeof(struct dentry));
	
	dentry->name = (char *)kernel_heap_alloc(string_len(name));
	memcpy((char *)dentry->name, name, sizeof(char) * string_len(name));
	dentry->inode = inode;
	
	return dentry;
}

struct vfs_mount *ramfs_mount(struct superblock *superblock, const char *path){
	struct vfs_mount *mount = (struct vfs_mount *)kernel_heap_alloc(sizeof(struct vfs_mount));
	
	mount->ops = &ops;
	mount->root = ramfs_create_inode(superblock, INODE_REGULAR_FILE, mount, NULL, 0, NULL); /* regular file for now */
	mount->mount_entry = mount_create_entry(path, "ramfs", "", superblock);
	
	return mount;
}

void ramfs_create(struct superblock *superblock, char *path, enum inode_type type, struct vfs_mount *mount, struct inode *last_inode){
	struct inode *inode = ramfs_create_inode(superblock, type, mount, NULL, 0, last_inode);
	struct dentry *dentry = ramfs_create_dentry(inode, path);
	
	inode->reference_count++;
	
	dentry_cache_new(path, dentry);
}

void ramfs_read_file(char *path, void *buf, size_t size){
	struct dentry *dentry = dentry_cache_search(path);
	
	if(buf == NULL || dentry->inode->data == NULL) return;
	
	if(dentry == NULL) return;
	
	memcpy(buf, dentry->inode->data, size);
}

void ramfs_write_file(char *path, void *buf, size_t size){
	struct dentry *dentry = dentry_cache_search(path);
	
	if(dentry->inode->data == NULL){
		dentry->inode->data = kernel_heap_alloc(size);
	}
	
	if(dentry == NULL) return;
	
	memcpy(dentry->inode->data, buf, size);
}
