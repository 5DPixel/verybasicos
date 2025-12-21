#include <stdint.h>
#include <kernel.h>
#include <kernel/framebuffer.h>
#include <kernel/psf.h>
#include <kernel/mm/rb_tree_alloc.h>
#include <kernel/mm/heap.h>

void kernel_init(struct kernel_boot_params *params){
	struct text_attributes attr = {0};
	attr.fg = COLOUR_RGB32(255, 255, 255);
	attr.bg = COLOUR_RGB32(0, 0, 0);
	attr.x = params->fb->width / 16;
	attr.y = params->fb->height / 16;
	clear_screen(params->fb, COLOUR_RGB32(0, 0, 0));

	kernel_heap_init(KERNEL_PAGE_SIZE_4K);
	char *str = kernel_heap_alloc_pages(1);
	str[0] = 'H';
	str[1] = 'I';
	str[2] = '\0';
	kernel_heap_free_pages(1, (void *)str);
	char *str2 = kernel_heap_alloc_pages(1);

	display_psf_string(params->fb, params->font, str2, &attr);
	for(;;); 
}
