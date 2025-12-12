#ifndef BOOT_SHARED_MENU_H
#define BOOT_SHARED_MENU_H

#include <stdint.h>
#include <boot/shared/platform.h>
#include <boot/shared/psf.h>

struct boot_menu_entry {
	const char *entry_name;
	void (*do_entry)(struct platform_model *platform);
};

struct boot_menu_attributes {
	int pad_x;
	int pad_y;
	struct text_attributes text_attr; 
};

void render_menu_entries(struct platform_model *model, struct boot_menu_entry *entries, int boot_entry_count, int selected_index, struct boot_menu_attributes *attr);

#endif
