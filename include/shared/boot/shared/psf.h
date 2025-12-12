#ifndef BOOT_SHARED_PSF_H
#define BOOT_SHARED_PSF_H

#include <boot/shared/platform.h>
#include <psf.h>

struct text_attributes {
	int x;
	int y;
	int bg;
	int fg;
};

void display_psf1_char(struct platform_model *model, uint8_t *psf, char ch, struct text_attributes *attr);
void display_psf1_char(struct platform_model *model, uint8_t *psf, char ch, struct text_attributes *attr);
void display_psf2_char(struct platform_model *model, uint8_t *psf, char ch, struct text_attributes *attr);
void display_psf2_string(struct platform_model *model, uint8_t *psf, char *str, struct text_attributes *attr);
void display_psf_string(struct platform_model *model, uint8_t *psf, char *str, struct text_attributes *attr);
enum psf_font_type font_type(uint8_t *psf);
int font_height(uint8_t *psf);
int font_width(uint8_t *psf);

#endif
