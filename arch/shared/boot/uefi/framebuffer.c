#include <boot/uefi/framebuffer.h>
#include <efi.h>

void locate_gop(struct uefi_boot_resources *resources){
	EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

	EFI_STATUS status;
	status = resources->gBS->LocateProtocol(&gop_guid, NULL, (void **)&gop);
	resources->gop = gop;
	fail_if(status != 0, L"Unable to locate GOP!", resources->gST);
}

EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **enumerate_gop_modes(struct uefi_boot_resources *resources){
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **info;
	EFI_STATUS status;
	UINTN size, modes;
	int i;

	modes = resources->gop->Mode->MaxMode;

	resources->gBS->AllocatePool(EfiLoaderCode, (UINTN)modes * sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION), (void **)&info);
	status = resources->gop->QueryMode(resources->gop, resources->gop->Mode == NULL ? 0: resources->gop->Mode->Mode, &size, &info[0]);
	
	for(i = 0; i < (int)modes; i++){
		status = resources->gop->QueryMode(resources->gop, i, &size, &info[i]);
		fail_if(status != 0, L"Failed to query GOP mode!\r\n", resources->gST);
	}

	return info;
}

UINT32 choose_gop_mode(struct uefi_boot_resources *resources){
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **info = enumerate_gop_modes(resources);
	UINTN modes;
	int i;

	UINT32 best_candidate_rgb = 0;
	UINT32 best_candidate_bgr = 0;
	UINT32 best_candidate_max = 0;

	modes = resources->gop->Mode->MaxMode;

	/* rank gop formats */

	for(i = 0; i < (int)modes; i++){
		switch(info[i]->PixelFormat){
			case PixelRedGreenBlueReserved8BitPerColor: {
				if(best_candidate_rgb == 0) best_candidate_rgb = i;
				if(CHECK_FRAMEBUFFER_RANKING(info[i], info[best_candidate_rgb])){
					best_candidate_rgb = i;
				}
				break;
			}

			case PixelBlueGreenRedReserved8BitPerColor: {
				if(best_candidate_bgr == 0) best_candidate_bgr = i;
				if(CHECK_FRAMEBUFFER_RANKING(info[i], info[best_candidate_bgr])){
					best_candidate_bgr = i;
				}
				break;
			}

			case PixelFormatMax: {
				if(best_candidate_max == 0) best_candidate_max = i;
				if(CHECK_FRAMEBUFFER_RANKING(info[i], info[best_candidate_max])){
					best_candidate_max = i;
				}
				break;
			}

			default: {
				break;
			}
		}
	}

	resources->gBS->FreePool(info);

	/* preferably choose RGB */
	if(best_candidate_rgb != 0) return best_candidate_rgb;
	if(best_candidate_bgr != 0) return best_candidate_bgr;
	if(best_candidate_max != 0) return best_candidate_max;

	return 0;
}

void set_gop_mode(struct uefi_boot_resources *resources){
	UINT32 graphics_mode = choose_gop_mode(resources);
	EFI_STATUS status;
	struct framebuffer *fb;
	resources->gBS->AllocatePool(EfiLoaderCode, sizeof(struct framebuffer), (void **)&fb);

	status = resources->gop->SetMode(resources->gop, graphics_mode);

	fail_if(status != 0, L"Failed to set GOP mode!\r\n", resources->gST);

	fb->base = resources->gop->Mode->FrameBufferBase;
	fb->size = resources->gop->Mode->FrameBufferSize;

	fb->width = resources->gop->Mode->Info->HorizontalResolution;
	fb->height = resources->gop->Mode->Info->VerticalResolution;
	fb->pitch = 4 * resources->gop->Mode->Info->PixelsPerScanLine;

	switch(resources->gop->Mode->Info->PixelFormat){
		case PixelRedGreenBlueReserved8BitPerColor: {
			fb->bpp = 8 * 4; /* 8bpc, rgba */
			break;
		}
		
		case PixelBlueGreenRedReserved8BitPerColor: {
			fb->bpp = 8 * 4; /* 8 bpc, bgra */
			break;
		}

		default: {
			fb->bpp = 8 * 4; /* default to 32bpp */
			break;
		}
	}

	resources->fb = fb;
}
