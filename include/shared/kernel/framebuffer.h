#ifndef KERNEL_FRAMEBUFFER_H
#define KERNEL_FRAMEBUFFER_H

#include <common.h>
#include <stdint.h>
#include <stddef.h>

struct text_attributes;

static inline void plot_pixel_32bpp(struct framebuffer *fb, int x, int y, uint32_t colour){
	*((uint32_t *)(fb->base + fb->pitch * y + 4 * x)) = colour;
}

void clear_screen(struct framebuffer *fb, uint32_t colour);

#endif
