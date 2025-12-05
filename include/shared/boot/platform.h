#ifndef _BOOT_PLATFORM_H
#define _BOOT_PLATFORM_H

#include <stddef.h>
#include <stdint.h>

#define PLATFORM_BLUE 0x01
#define PLATFORM_WHITE 0x0F

#define TEXT_ATTR(fg, bg) fg | (bg >> 8)
#define BOOT_MENU_TEXT_ATTR TEXT_ATTR(PLATFORM_WHITE, PLATFORM_BLUE)

typedef struct {
	void *(*alloc)(void *ctx, size_t size);
	void (*free)(void *ctx, void *ptr, size_t size);
	void (*log)(void *ctx, const char *message);
	void (*open_file)(void *ctx, const char *file_name);
	void *ctx;
} platform_model;

void *alloc(void *ctx, size_t size);
void free(void *ctx, void *ptr, size_t size);
void log(void *ctx, const char *message);
void set_text_attr(void *ctx, uint32_t text_attr);

#endif
