#include <boot/menu.h>

void init_menu(boot_resources *resources){
    resources->gBS->SetWatchdogTimer(0, 0, 0, NULL);
    resources->gST->ConOut->SetAttribute(resources->gST->ConOut, MENU_SCREEN_COLOUR);
    resources->gST->ConOut->ClearScreen(resources->gST->ConOut);
	resources->gST->ConOut->SetCursorPosition(resources->gST->ConOut, 0, 0);
}

void render_boot_menu(boot_resources *resources, boot_menu_entry *menu_entries, int menu_entry_count){
	int i;
    resources->gST->ConOut->ClearScreen(resources->gST->ConOut);
	resources->gST->ConOut->SetCursorPosition(resources->gST->ConOut, 0, 0);
	resources->gST->ConOut->OutputString(resources->gST->ConOut, L"Select boot option\r\n\r\n");
	for(i = 0; i < menu_entry_count; i++){
		if(i == resources->menu_entry_index)
			resources->gST->ConOut->SetAttribute(resources->gST->ConOut, SELECTED_TEXT_COLOUR);
			
		resources->gST->ConOut->OutputString(resources->gST->ConOut, menu_entries[i].entry_name);
		resources->gST->ConOut->SetAttribute(resources->gST->ConOut, MENU_SCREEN_COLOUR);
	}
}
