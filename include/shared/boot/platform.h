#ifndef _BOOT_PLATFORM_H
#define _BOOT_PLATFORM_H

#include <stddef.h>
#include <stdint.h>

/* common colours */
#define PLATFORM_WHITE 0xFFFFFFFF
#define PLATFORM_BLACK 0x00000000

#define TEXT_ATTR(fg, bg) fg | (bg >> 8)
#define BOOT_MENU_TEXT_ATTR TEXT_ATTR(PLATFORM_WHITE, PLATFORM_BLUE)

struct text_attributes;

struct platform_model {
	void *(*alloc)(struct platform_model *model, size_t size);
	void (*free)(struct platform_model *model, void *ptr, size_t size);
	void (*log)(struct platform_model *model, const char *message, struct text_attributes attr, uint8_t *psf);
	uint8_t *(*read_file)(struct platform_model *model, const char *file_name);
	void (*plot32)(struct platform_model *model, int x, int y, uint32_t colour);
	void *ctx;
};

void *platform_alloc(struct platform_model *model, size_t size);
void platform_free(struct platform_model *model, void *ptr, size_t size);
void platform_log(struct platform_model *model, const char *message, struct text_attributes attr, uint8_t *psf);
uint8_t *platform_read_file(struct platform_model *model, const char *file_name);
void set_text_attr(struct platform_model *model, uint32_t text_attr);
void plot_pixel_32bpp(struct platform_model *model, int x, int y, uint32_t colour);
void init_platform(void *ctx, struct platform_model *model);
void open_font_file(struct platform_model *model, const char *file);

#endif
