#ifndef UEFI_BOOT_FRAMEBUFFER_H
#define UEFI_BOOT_FRAMEBUFFER_H

#include <efi.h>
#include <boot/uefi/common.h>

#define CHECK_FRAMEBUFFER_RANKING(a, b) a->HorizontalResolution * a->VerticalResolution > \
	b->HorizontalResolution * b->VerticalResolution

void locate_gop(struct uefi_boot_resources *resources);
EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **enumerate_gop_modes(struct uefi_boot_resources *resources);
void set_gop_mode(struct uefi_boot_resources *resources);

#endif
