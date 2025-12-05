#include <boot/platform.h>
#include <boot/uefi/common.h>
#include <common.h>
#include <efi.h>

// prefix with __ to signify internal, -DPLATFORM_EFI in my CPPFLAGS and use a #ifdef
void *alloc(void *ctx, size_t size){
	// expect ctx to be the boot_resources struct
	void *buffer;
	boot_resources *resources = ctx;
	resources->gBS->AllocatePool(EfiLoaderCode, (UINTN)size, (void **)&buffer);
	
	return buffer;
}

void free(void *ctx, void *ptr, size_t size){
	
}

void log(void *ctx, const char *message){
	boot_resources *resources = ctx;
	int message_len = string_len(message);
	short unsigned int *wstr = (short unsigned int *)alloc(ctx, message_len); /* 2 to account for the CR + LF */
	string_to_wstring(message, wstr);
	resources->gST->ConOut->OutputString(resources->gST->ConOut, wstr);
}
