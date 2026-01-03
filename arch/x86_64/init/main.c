#include <stdint.h>
#include <kernel.h>
#include <kernel/framebuffer.h>
#include <kernel/psf.h>
#include <kernel/mm/heap.h>
#include <kernel/vfs/file.h>
#include <kernel/vfs/mount.h>
#include <kernel/fs/ramfs.h>
#include <kernel/list/hash.h>
#include <kernel/vfs/dentry_cache.h>

void kernel_init(struct kernel_boot_params *params){
	struct text_attributes attr = {0};
	attr.fg = COLOUR_RGB32(255, 255, 255);
	attr.bg = COLOUR_RGB32(0, 0, 0);
	attr.x = params->fb->width / 32;
	attr.y = params->fb->height / 32;
	clear_screen(params->fb, COLOUR_RGB32(0, 0, 0));

	kernel_heap_init(KERNEL_PAGE_SIZE_4K);
	
	init_vfs_types();

	struct superblock ramfs_superblock = {0};
	
	ramfs_init(&ramfs_superblock);
	mount_init();
	
	dentry_cache_init();
	
	const char *file_name = "/hello.txt";
	const char *test = "hello";
	void *read = kernel_heap_alloc(string_len(test));
	struct vfs_mount *mount = ramfs_mount(&ramfs_superblock, "/");
	//ramfs_create(&ramfs_superblock, (char *)file_name, INODE_REGULAR_FILE, mount, NULL);
	//ramfs_write_file((char *)file_name, (void *)test, string_len((char *)test));
	//ramfs_read_file((char *)file_name, (void *)read, string_len((char *)test));
	
	display_psf_string(params->fb, params->font, "Hello, world!", &attr);
	for(;;); 
}
