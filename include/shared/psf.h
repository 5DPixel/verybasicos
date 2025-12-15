#ifndef SHARED_PSF_H
#define SHARED_PSF_H

#include <stdint.h>

#define PSF1_FONT_MAGIC 0x0436
#define PSF2_FONT_MAGIC 0x864ab572

struct text_attributes {
    int x;
    int y;
    int bg;
    int fg;
};

enum psf_font_type {
	FONT_PSF1 = 0,
	FONT_PSF2,
	FONT_UNKNOWN
};

struct psf1_font_header {
	uint16_t magic;
	uint8_t font_mode;
	uint8_t char_size;
} __attribute__((packed));

struct psf2_font_header {
	uint32_t magic;
	uint32_t version;
	uint32_t header_size;
	uint32_t flags;
	uint32_t glyphs;
	uint32_t bytes_per_glyph;
	uint32_t height;
	uint32_t width;
};

#endif
