#include <stdint.h>
#include <kernel.h>
#include <kernel/framebuffer.h>
#include <kernel/psf.h>

void kernel_init(struct kernel_boot_params *params){
	struct text_attributes attr = {0};
	attr.fg = COLOUR_RGB32(255, 255, 255);
	attr.bg = COLOUR_RGB32(0, 0, 0);
	attr.x = params->fb->width / 16;
	attr.y = params->fb->height / 16;
	clear_screen(params->fb, COLOUR_RGB32(0, 0, 0));

	display_psf_string(params->fb, params->font, "String", &attr);
	for(;;); 
}
