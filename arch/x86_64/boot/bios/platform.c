#include <boot/shared/platform.h>
#include <boot/shared/psf.h>
#include <common.h>
#include <kernel/const.h>

const char *alloc_start = (const char*)0x7C00 + 1474560; /* 0x7C00 + sizeof a floppy */

/* provide very primitive definitions sufficient to get into the bootloader's next stage */
void *platform_alloc(struct platform_model *model, size_t size){
	(void)model;
	alloc_start += size;
	return (void *)alloc_start;
}

/* copy over, nothing new */
void *platform_alloc_conventional(struct platform_model *model, size_t size){
	(void)model;
	alloc_start += size;
	return (void *)alloc_start;
}

/* literally nothing needed here */
void platform_alloc_pages(struct platform_model *model, int page_count, uint64_t addr){
	(void)model;
	(void)page_count;
	(void)addr;
}

void platform_free(struct platform_model *model, void *ptr){
	(void)model;
	(void)ptr;
	/* TODO: free */
}
