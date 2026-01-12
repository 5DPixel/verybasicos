#include <elf.h>
#include <common.h>
#include <boot/shared/platform.h>
#include <boot/shared/psf.h>
#include <boot/shared/menu.h>
#include <boot/shared/common.h>
#include <boot/shared/boot.h>
#include <psf.h>

void boot_main(struct platform_model *model){
	int font_width, font_height;
	model->font_dimensions(model, &font_width, &font_height);

    struct framebuffer *fb = platform_display_attributes(model);
    struct text_attributes attr = {0};
    attr.x = fb->width / 8; // divide by 16 instead for log position
    attr.y = fb->height / 8;
    attr.fg = PLATFORM_WHITE;
    attr.bg = PLATFORM_BLACK;

	struct boot_menu_entry default_boot = {0};
	default_boot.entry_name = "Default Boot\r";
	default_boot.do_entry = start_default_boot;

	struct boot_menu_entry suppress_logs = {0};
	suppress_logs.entry_name = "Default Boot (Suppressed logs)\r";

	struct boot_menu_entry entries[] = { default_boot, suppress_logs };

	struct boot_menu_attributes menu_attr;
	menu_attr.pad_y = font_height * 2;
	menu_attr.pad_x = 0;
	menu_attr.text_attr = attr;

	int menu_entry_count = 2;

	int selected_index = 0;
	render_menu_entries(model, entries, menu_entry_count, selected_index, &menu_attr);

	enum key pressed_key;
	
	while(1){
		pressed_key = model->get_key(model);

		switch(pressed_key){
			case KEY_UP_ARROW:
				if(selected_index == 0) break;
				selected_index--;
				render_menu_entries(model, entries, menu_entry_count, selected_index, &menu_attr);
				break;

			case KEY_DOWN_ARROW:
				if(selected_index == menu_entry_count - 1) break;
				selected_index++;
				render_menu_entries(model, entries, menu_entry_count, selected_index, &menu_attr);
				break;
			
			case KEY_SPACE:
				if(entries[selected_index].do_entry == NULL) break;
				entries[selected_index].do_entry(model);
				break;
		}
	}
}
