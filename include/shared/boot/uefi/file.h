#ifndef _UEFI_FILE_H
#define _UEFI_FILE_H

#include <efi.h>
#include <boot/uefi/common.h>

void get_root_volume(boot_resources *resources);
void open_kernel_file(boot_resources *resources, CHAR16 *file_name);
void read_kernel_file(boot_resources *resources);
UINT64 file_size(boot_resources *resources, EFI_FILE_HANDLE file);

#endif
