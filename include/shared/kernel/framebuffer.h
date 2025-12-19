#ifndef KERNEL_FRAMEBUFFER_H
#define KERNEL_FRAMEBUFFER_H

#include <common.h>
#include <stdint.h>
#include <stddef.h>

struct text_attributes;

static inline void plot_pixel_32bpp(struct framebuffer *fb, int x, int y, uint32_t colour){
	*((uint32_t *)(fb->base + fb->pitch * y + 4 * x)) = colour;
}

static inline void clear_screen(struct framebuffer *fb, uint32_t colour){
    int i;
    for(i = 0; i < (int)(fb->width * fb->height); i++)
        plot_pixel_32bpp(fb, (i % fb->width), (i / fb->width), colour);
}

void clear_screen(struct framebuffer *fb, uint32_t colour);

#endif
