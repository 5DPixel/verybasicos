#include <stdint.h>
#include <kernel.h>
#include <kernel/framebuffer.h>
#include <kernel/psf.h>
#include <kernel/mm/rb_tree_alloc.h>

void kernel_init(struct kernel_boot_params *params){
	struct text_attributes attr = {0};
	attr.fg = COLOUR_RGB32(255, 255, 255);
	attr.bg = COLOUR_RGB32(0, 0, 0);
	attr.x = params->fb->width / 16;
	attr.y = params->fb->height / 16;
	clear_screen(params->fb, COLOUR_RGB32(0, 0, 0));
	
	char initial_data[4096];
	
	struct rb_tree_allocator alloc = {0};
	rb_tree_alloc_init(&alloc, PAGE_SIZE_4K, (void *)initial_data);
	
	char *str = (char *)rb_tree_alloc_pages(&alloc, 1, 0);
	str[0] = 'H';
	str[1] = 'I';
	str[2] = '\0';

	display_psf_string(params->fb, params->font, str, &attr);
	for(;;); 
}
