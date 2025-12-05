#ifndef _BOOT_MENU_H
#define _BOOT_MENU_H

#include <efi.h>
#include <boot/common.h>

#define MENU_SCREEN_COLOUR EFI_TEXT_ATTR(EFI_WHITE, EFI_BLUE)
#define SELECTED_TEXT_COLOUR EFI_TEXT_ATTR(EFI_BLUE, EFI_CYAN)

typedef struct {
    CHAR16 *entry_name;
    void (*do_entry)(boot_resources *resources);
} boot_menu_entry;

void init_menu(boot_resources *resources);
void render_boot_menu(boot_resources *resources, boot_menu_entry *menu_entries, int menu_entry_count);

#endif
