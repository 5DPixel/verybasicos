#include <stdint.h>
#include <kernel.h>
#include <kernel/framebuffer.h>

void kernel_init(struct kernel_boot_params *params){
	int i;
	for(i = 0; i < 1000; i++)
		plot_pixel_32bpp(params->fb, i, i, COLOUR_RGB32(255, 255, 255)); 

	return;
}
