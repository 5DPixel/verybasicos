#include <kernel/framebuffer.h>

void clear_screen(struct framebuffer *fb, uint32_t colour){
	int i;
	for(i = 0; i < (int)(fb->width * fb->height); i++)
		plot_pixel_32bpp(fb, (i % fb->width), (i / fb->width), colour);
}
