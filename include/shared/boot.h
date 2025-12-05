#ifndef _BOOT_H
#define _BOOT_H

#include <stdint.h>

typedef enum {
	color_rgb_reserved_8bpc,
	color_bgr_reserved_8bpc,
	color_bit_mask,
	color_blt_only,
	color_max
} efi_pixel_format;

typedef struct {
	uint32_t red;
	uint32_t green;
	uint32_t blue;
	uint32_t reserved;
} efi_pixel_bitmask;

typedef struct {
	uint32_t version;
	uint32_t horizontal_resolution;
	uint32_t vertical_resolution;
	efi_pixel_format pixel_format;
	efi_pixel_bitmask pixel_information;
	uint32_t pixels_per_scanline;
} efi_framebuffer_info;

typedef struct {
	uint32_t max_mode;
	uint32_t mode;
	efi_framebuffer_info *info;
	uint64_t framebuffer_info_size;
	uint64_t framebuffer_base;
	uint64_t framebuffer_size;
} efi_framebuffer;

#endif
