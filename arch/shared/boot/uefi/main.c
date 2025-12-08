#include <efi.h>
#include <elf.h>
#include <boot/const.h>
#include <boot/uefi/file.h>
#include <common.h>
#include <boot/uefi/common.h>
#include <boot/menu.h>
#include <boot/platform.h>
#include <boot/uefi/framebuffer.h>
#include <boot/psf.h>
#include <psf.h>

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	(void)ImageHandle; /* cast to a void, because I'm aware some compilers will warn as the ImageHandle is unused */

	boot_resources resources;
	struct platform_model model;

	resources.gST = SystemTable;
	resources.gBS = SystemTable->BootServices;
	resources.gRT = SystemTable->RuntimeServices;
	resources.image = ImageHandle;
	resources.menu_entry_index = 0;

	init_platform((void *)&resources, &model);
	locate_gop(&resources);
	set_gop_mode(&resources);
	int i;
	uint8_t *font = model.read_file(&model, "EFI\\BOOT\\spleen.psf");
	//psf1_font_header header = {0};
	struct text_attributes attr = {0};
	attr.x = 100;
	attr.y = 100;
	attr.fg = PLATFORM_BLACK;
	attr.bg = PLATFORM_WHITE;
	//model.log(&model, "Hello, world", &attr, font);
	//display_psf2_char(&model, font, 'A', &attr);
	display_psf_string(&model, font, "Hello, world", attr);

	//return EFI_SUCCESS;
	//for(i = 0; i < 1000; i++)
		//model.plot32(&model, i, i, create_colour(&resources, 255, 0, 0)); /* red */
	for(;;)

	//init_menu(&resources);

	get_root_volume(&resources);
	open_kernel_file(&resources, L"EFI\\BOOT\\kernel");
	read_kernel_file(&resources);

	boot_menu_entry boot_normal = {
		.entry_name = L"Default Boot\r\n",
		.do_entry = NULL
	};

	boot_menu_entry boot_suppress_logs = {
		.entry_name = L"Default Boot (with suppressed logging)\r\n",
		.do_entry = NULL
	};

	boot_menu_entry shutdown = {
		.entry_name = L"Shutdown system\r\n",
		.do_entry = shutdown_system
	};

	boot_menu_entry boot_entries[] = { boot_normal, boot_suppress_logs, shutdown };
	int boot_entry_count = 3;

	render_boot_menu(&resources, boot_entries, boot_entry_count);

	EFI_INPUT_KEY key;

	while(1){
		while(resources.gST->ConIn->ReadKeyStroke(resources.gST->ConIn, &key));

		switch(key.ScanCode){
			case SCANCODE_UP_ARROW:
				if(resources.menu_entry_index == 0) break;
				resources.menu_entry_index--;
				render_boot_menu(&resources, boot_entries, boot_entry_count);
				break;
				
			default:
				if(key.UnicodeChar != L'\r' || boot_entries[resources.menu_entry_index].do_entry == NULL) break;
				boot_entries[resources.menu_entry_index].do_entry(&resources);
				break;

			case SCANCODE_DOWN_ARROW:
				if(resources.menu_entry_index == boot_entry_count - 1) break;
				resources.menu_entry_index++;
				render_boot_menu(&resources, boot_entries, boot_entry_count);
				break;
		}
	}

 	return EFI_SUCCESS;
}
