#include <common.h>
#include <boot/uefi/common.h>
#include <boot/shared/common.h>

void fail_if(int condition, CHAR16 *wstr, EFI_SYSTEM_TABLE *gST){
    if(!condition) return;

    gST->ConOut->OutputString(gST->ConOut, wstr);

    goto cleanup;

	cleanup:
    	return;
}

void shutdown_system(struct uefi_boot_resources *resources){
	resources->gRT->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
}

uint32_t create_colour(struct uefi_boot_resources *resources, uint32_t r, uint32_t g, uint32_t b){
	if(resources->gop->Mode->Info->PixelFormat == PixelBlueGreenRedReserved8BitPerColor)
		return COLOUR_BGR32(b, g, r);
	
	return COLOUR_RGB32(r, g, b);
}
