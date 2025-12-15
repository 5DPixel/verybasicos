#ifndef KERNEL_PSF_H
#define KERNEL_PSF_H

#include <psf.h>
#include <stdint.h>
#include <common.h>

static inline enum psf_font_type font_type(uint8_t *psf){
    if(*(uint16_t *)psf == PSF1_FONT_MAGIC) return FONT_PSF1;
    if(*(uint32_t *)psf == PSF2_FONT_MAGIC) return FONT_PSF2;

    return FONT_UNKNOWN;
}

void display_psf1_char(struct framebuffer *fb, uint8_t *psf, char ch, struct text_attributes *attr);
void display_psf1_string(struct framebuffer *fb, uint8_t *psf, char *str, struct text_attributes *attr);
void display_psf2_char(struct framebuffer *fb, uint8_t *psf, char ch, struct text_attributes *attr);
void display_psf2_string(struct framebuffer *fb, uint8_t *psf, char *str, struct text_attributes *attr);
void display_psf_string(struct framebuffer *fb, uint8_t *psf, char *str, struct text_attributes *attr);

#endif
