#include <efi.h>
#include <elf.h>
#include <boot/const.h>
#include <boot/uefi/file.h>
#include <common.h>
#include <boot/uefi/common.h>
#include <boot/menu.h>
#include <boot/platform.h>
#include <boot/uefi/framebuffer.h>

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	(void)ImageHandle; /* cast to a void, because I'm aware some compilers will warn as the ImageHandle is unused */

	EFI_FILE_HANDLE root_volume;
	EFI_FILE_HANDLE kernel_file;
	boot_resources resources;

	resources.gST = SystemTable;
	resources.gBS = SystemTable->BootServices;
	resources.gRT = SystemTable->RuntimeServices;
	resources.image = ImageHandle;
	resources.menu_entry_index = 0;

	locate_gop(&resources);
	set_gop_mode(&resources);
	//enumerate_gop_modes(&resources);
	return EFI_SUCCESS;

	init_menu(&resources);

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
