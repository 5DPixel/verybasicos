#include <efi.h>
#include <elf.h>
#include <boot/uefi/const.h>
#include <common.h>
#include <boot/uefi/common.h>
#include <boot/shared/platform.h>
#include <boot/uefi/framebuffer.h>
#include <boot/shared/psf.h>
#include <psf.h>
#include <boot.h>

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	(void)ImageHandle; /* cast to a void, because I'm aware some compilers will warn as the ImageHandle is unused */

	struct uefi_boot_resources resources;
	struct platform_model model;
	const char *font_file_path = "EFI\\BOOT\\spleen.psf";
	const char *kernel_file_path = "EFI\\BOOT\\kernel";

	resources.gST = SystemTable;
	resources.gBS = SystemTable->BootServices;
	resources.gRT = SystemTable->RuntimeServices;
	resources.image = ImageHandle;

	locate_gop(&resources);
	set_gop_mode(&resources);
	init_platform((void *)&resources, font_file_path, kernel_file_path, &model);
	boot_main(&model);

	return EFI_SUCCESS;
}
