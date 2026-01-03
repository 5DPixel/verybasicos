#ifndef KERNEL_SHARED_VFS_FILE_H
#define KERNEL_SHARED_VFS_FILE_H

#include <stddef.h>
#include <stdint.h>

struct superblock;

enum vfs_mount_flags {
	VFS_MOUNT_READONLY = 0
};

struct vfs_ops {
	struct vfs_mount *(*mount)(struct superblock *, const char *); /* mount name and fs type */
	void (*unmount)(const char *);
	void (*sync)();
};

struct vfs_mount {
	struct vfs_ops *ops;
	struct inode *root;
	struct mount_entry *mount_entry;
};

struct vfs_type {
	const char *name;
	struct vfs_type *next;
};

enum inode_type {
	INODE_REGULAR_FILE = 0,
	INODE_DIRECTORY = 1,
	INODE_CHAR_DEVICE = 2,
	INODE_BLOCK_DEVICE = 3
};

struct inode_file_ops {
	void (*read)(char *, void *, size_t); /* void * -- buffer to read/write to/from */
	void (*write)(char *, void *, size_t);
};

struct inode_ops {
	void (*create)(struct superblock *, char *, enum inode_type, struct vfs_mount *, struct inode *);
};

struct inode {
	enum inode_type type;
	struct inode_file_ops *file_ops;
	int reference_count;
	int pending_writes;
	struct vfs_mount *mount;
	void *data; /* fs-specific data */
	size_t data_size;
	struct inode *next;
	struct superblock *superblock;
};

struct dentry {
	char *name;
	struct inode *inode;
};

struct superblock {
	uint64_t inode_count;
	/* rb tree to each inode */
};

void init_vfs_types();
void register_vfs_type(struct vfs_type *type);
struct vfs_type *find_vfs_type(const char *name);

#endif
