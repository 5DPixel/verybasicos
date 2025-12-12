#ifndef UEFI_BOOT_COMMON_H
#define UEFI_BOOT_COMMON_H

#include <efi.h>
#include <common.h>

struct uefi_boot_resources {
	EFI_HANDLE image;
	EFI_SYSTEM_TABLE *gST;
	EFI_RUNTIME_SERVICES *gRT;
	EFI_BOOT_SERVICES *gBS;
	EFI_FILE_HANDLE root_volume;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
	UINTN mmap_key;
	struct framebuffer *fb;
};

void fail_if(int condition, CHAR16 *wstr, EFI_SYSTEM_TABLE *gST);
void shutdown_system(struct uefi_boot_resources *resources);
uint32_t create_colour(struct uefi_boot_resources *resources, uint32_t r, uint32_t g, uint32_t b);

#endif
