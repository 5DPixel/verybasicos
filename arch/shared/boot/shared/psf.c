#include <boot/psf.h>

enum psf_font_type font_type(uint8_t *psf){
	//TODO: fail if incorrect, potentially in serial console
	if(*(uint16_t *)psf == PSF1_FONT_MAGIC) return FONT_PSF1;
	if(*(uint32_t *)psf == PSF2_FONT_MAGIC) return FONT_PSF2;

	return FONT_UNKNOWN;
}

void display_psf1_char(struct platform_model *model, uint8_t *psf, char ch, struct text_attributes *attr){
	struct psf1_font_header *header = (struct psf1_font_header *)psf;
	int i, j;
	uint8_t *row = psf + ch * header->char_size + sizeof(struct psf1_font_header);

	for(i = 0; i < header->char_size; i++){
		for(j = 0; j < 8; j++){
			if(*row & (0x80 >> j))
				model->plot32(model, attr->x + j, attr->y + i, attr->fg);
			else
				model->plot32(model, attr->x + j, attr->y + i, attr->bg);
		}

		row++;
	}
}

void display_psf1_string(struct platform_model *model, uint8_t *psf, char *str, struct text_attributes attr){
	int i;
	for(i = 0; str[i] != '\0'; i++){
		display_psf1_char(model, psf, str[i], &attr);
		attr.x += 8;
	}
}

void display_psf2_char(struct platform_model *model, uint8_t *psf, char ch, struct text_attributes *attr){
	struct psf2_font_header *header = (struct psf2_font_header *)psf;
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
				model->plot32(model, attr->x + j, attr->y + i, attr->fg);
			else
				model->plot32(model, attr->x + j, attr->y + i, attr->bg);

			bit--;
			if(bit < 0){
				bit = 7;
				byte++;
			}
		}
	}
}

void display_psf2_string(struct platform_model *model, uint8_t *psf, char *str, struct text_attributes attr){
	struct psf2_font_header *header = (struct psf2_font_header *)psf;
	int i;
	for(i = 0; str[i] != '\0'; i++){
		display_psf2_char(model, psf, str[i], &attr);
		attr.x += header->width;
	}
}

void display_psf_string(struct platform_model *model, uint8_t *psf, char *str, struct text_attributes attr){
	if(font_type(psf) == FONT_PSF1){
		display_psf1_string(model, psf, str, attr);
	} else if(font_type(psf) == FONT_PSF2)
		display_psf2_string(model, psf, str, attr);
}
