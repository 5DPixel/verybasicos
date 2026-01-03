#ifndef KERNEL_SHARED_FS_RAMFS_H
#define KERNEL_SHARED_FS_RAMFS_H

#include <stddef.h>
#include <kernel/vfs/mount.h>

enum ramfs_file_type {
	RAMFS_REGULAR_FILE = 0,
	RAMFS_DIRECTORY = 1,
	RAMFS_SYMLINK = 2,
	RAMFS_CHAR_DEVICE = 3,
	RAMFS_BLOCK_DEVICE = 4
};

void ramfs_init(struct superblock *superblock);
struct inode *ramfs_create_inode(struct superblock *superblock, enum inode_type type, struct vfs_mount *mount, void *data, size_t data_size, struct inode *last);
struct dentry *ramfs_create_dentry(struct inode *inode, char *name);
void ramfs_create(struct superblock *superblock, char *path, enum inode_type type, struct vfs_mount *mount, struct inode *last_inode);
void ramfs_read_file(char *path, void *buf, size_t size);
void ramfs_write_file(char *path, void *buf, size_t size);
struct vfs_mount *ramfs_mount(struct superblock *superblock, const char *path);

#endif
