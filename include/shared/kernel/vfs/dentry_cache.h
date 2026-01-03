#ifndef KERNEL_SHARED_VFS_DENTRY_CACHE_H
#define KERNEL_SHARED_VFS_DENTRY_CACHE_H

#include <kernel/vfs/file.h>

void dentry_cache_init();
void dentry_cache_new(char *file_name, struct dentry *dentry);
struct dentry *dentry_cache_search(char *file_name);

#endif
