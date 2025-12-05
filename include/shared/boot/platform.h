#ifndef _BOOT_PLATFORM_H
#define _BOOT_PLATFORM_H

#include <stddef.h>

typedef struct {
	void *(*alloc)(void *ctx, size_t size);
	void (*free)(void *ctx, void *ptr, size_t size);
	void (*log)(void *ctx, const char *message);
	void (*open_file)(void *ctx, const char *file_name);
	void *ctx;
} io_model;

void *uefi_alloc(void *ctx, size_t size);

#endif
