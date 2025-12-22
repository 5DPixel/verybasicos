#include <stdint.h>
#include <kernel.h>
#include <kernel/framebuffer.h>
#include <kernel/psf.h>
#include <kernel/mm/rb_tree_alloc.h>
#include <kernel/mm/heap.h>
#include <kernel/mm/slab.h>

void kernel_init(struct kernel_boot_params *params){
	struct text_attributes attr = {0};
	attr.fg = COLOUR_RGB32(255, 255, 255);
	attr.bg = COLOUR_RGB32(0, 0, 0);
	attr.x = params->fb->width / 16;
	attr.y = params->fb->height / 16;
	clear_screen(params->fb, COLOUR_RGB32(0, 0, 0));

	kernel_heap_init(KERNEL_PAGE_SIZE_4K);
	char *t = kernel_heap_alloc(16);
	t[0] = 'A';
	t[1] = '\0';
	
	kernel_heap_free((void *)t, 16);
	
	char *t2 = kernel_heap_alloc(16);
	
	//display_psf_string(params->fb, params->font, t, &attr);
	display_psf_string(params->fb, params->font, t2, &attr);
	for(;;); 
}
