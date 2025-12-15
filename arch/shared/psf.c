#include <kernel/psf.h>
#include <kernel/framebuffer.h>
#include <common.h>

/* use volatile; gcc optimised the header and framebuffer out for some reason */

void display_psf1_char(struct framebuffer *fb, uint8_t *psf, char ch, struct text_attributes *attr){
	struct psf1_font_header *header = (struct psf1_font_header *)psf;
	int i, j;
	uint8_t *row = psf + ch * header->char_size + sizeof(struct psf1_font_header);
	
	for(i = 0; i < header->char_size; i++){
		for(j = 0; j < 8; j++){
			if(*row & (0x80 >> j))
				plot_pixel_32bpp(fb, attr->x + j, attr->y + i, attr->fg);
			else
				plot_pixel_32bpp(fb, attr->x + j, attr->y + i, attr->bg);
		}
		row++;
	}
}

void display_psf1_string(struct framebuffer *fb, uint8_t *psf, char *str, struct text_attributes *attr){
    int i;
    for(i = 0; str[i] != '\0'; i++){
        if(str[i] == '\n'){
            attr->y += 16;
            attr->x -= 8 * i;
            continue;
        }
        if(str[i] == '\r'){
            attr->x -= 8 * i;
            continue;
        }
        display_psf1_char(fb, psf, str[i], attr);
        attr->x += 8;
    }
}

void display_psf2_char(struct framebuffer *fb, uint8_t *psf, char ch, struct text_attributes *attr){
    volatile struct psf2_font_header *header = (volatile struct psf2_font_header *)psf;
    int i, j;
    uint8_t *glyph = psf + header->header_size + ch * header->bytes_per_glyph;
    uint8_t bytes_per_line = (header->width + 7) / 8;
    int bit, byte;
    uint8_t *row;

    for(i = 0; i < (int)header->height; i++){
        row = glyph + i * bytes_per_line;
        bit = 7;
        byte = 0;

        for(j = 0; j < (int)header->width; j++){
            if(row[byte] & (1 << bit))
                plot_pixel_32bpp(fb, attr->x + j, attr->y + i, attr->fg);
            else
                plot_pixel_32bpp(fb, attr->x + j, attr->y + i, attr->bg);

            bit--;
            if(bit < 0){
                bit = 7;
                byte++;
            }
        }
    }
}

void display_psf2_string(struct framebuffer *fb, uint8_t *psf, char *str, struct text_attributes *attr){
    volatile struct psf2_font_header *header = (volatile struct psf2_font_header *)psf;
    int i;
    for(i = 0; str[i] != '\0'; i++){
        if(str[i] == '\n'){
            attr->y += header->height;
            attr->x -= header->width * i;
            continue;
        }
        if(str[i] == '\r'){
            attr->x -= header->width * i;
            continue;
        }
        display_psf2_char(fb, psf, str[i], attr);
        attr->x += header->width;
    }
}

void display_psf_string(struct framebuffer *fb, uint8_t *psf, char *str, struct text_attributes *attr){
    enum psf_font_type type = font_type(psf);

    if(type == FONT_PSF1){
        display_psf1_string(fb, psf, str, attr);
    } else if(type == FONT_PSF2)
        display_psf2_string(fb, psf, str, attr);
}
