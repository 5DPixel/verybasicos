#include <boot/shared/menu.h>

void render_menu_entries(struct platform_model *model, struct boot_menu_entry *entries, int boot_entry_count, int selected_index, struct boot_menu_attributes *attr){
	int i;

	struct text_attributes inverted = {0};
	inverted.bg = attr->text_attr.fg;
	inverted.fg = attr->text_attr.bg;
	inverted.x = attr->text_attr.x;
	inverted.y = attr->text_attr.y;

	for(i = 0; i < boot_entry_count; i++){
		if(i == selected_index){
			inverted.x = attr->text_attr.x;
			inverted.y = attr->text_attr.y;
			model->write(model, entries[i].entry_name, &inverted);
			attr->text_attr.x += attr->pad_x;
			attr->text_attr.y += attr->pad_y;
			continue;
		}
		model->write(model, entries[i].entry_name, &attr->text_attr);
		attr->text_attr.x += attr->pad_x;
		attr->text_attr.y += attr->pad_y;
	}

	attr->text_attr.x -= attr->pad_x * i;
	attr->text_attr.y -= attr->pad_y * i;
}
