#include <boot/platform.h>
#include <boot/common.h>
#include <efi.h>

void *uefi_alloc(void *ctx, size_t size){
	// expect ctx to be the boot_resources struct
	void *buffer;
	boot_resources *resources = ctx;
	resources->gBS->AllocatePool(EfiLoaderCode, (UINTN)size, (void **)buffer);
	
	return buffer;
}
