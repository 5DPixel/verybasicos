#ifndef _BOOT_COMMON_H
#define _BOOT_COMMON_H

#include <efi.h>
#include <common.h>

typedef struct {
	EFI_HANDLE image;
	EFI_SYSTEM_TABLE *gST;
	EFI_RUNTIME_SERVICES *gRT;
	EFI_BOOT_SERVICES *gBS;
	EFI_FILE_HANDLE root_volume;
	EFI_FILE_HANDLE kernel_file;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
	UINT8 *kernel_file_buffer;
	UINT64 kernel_file_length;
	int menu_entry_index;
	framebuffer *fb;
} boot_resources;

#define SCANCODE_UP_ARROW 0x01
#define SCANCODE_DOWN_ARROW 0x02

void fail_if(int condition, CHAR16 *wstr, EFI_SYSTEM_TABLE *gST);
void shutdown_system(boot_resources *resources);
uint32_t create_colour(boot_resources *resources, uint32_t r, uint32_t g, uint32_t b);

#endif
