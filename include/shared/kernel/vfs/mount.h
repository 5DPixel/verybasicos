#ifndef KERNEL_SHARED_VFS_MOUNT_H
#define KERNEL_SHARED_VFS_MOUNT_H

#include <kernel/vfs/file.h>

struct mount_entry {
	const char *mount_path;
	const char *fs_name;
	const char *dev_name;
	struct superblock *superblock;
};

void mount_init();
struct mount_entry *mount_create_entry(const char *mount_path, const char *fs_name, const char *dev_name, struct superblock *superblock);

#endif
