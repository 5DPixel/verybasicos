#include <boot/platform.h>
#include <boot/uefi/common.h>
#include <boot/psf.h>
#include <common.h>
#include <efi.h>

void *platform_alloc(struct platform_model *model, size_t size){
	// expect ctx to be the boot_resources struct
	void *buffer;
	boot_resources *resources = model->ctx;
	resources->gBS->AllocatePool(EfiLoaderCode, (UINTN)size, (void **)&buffer);
	
	return buffer;
}

void platform_free(struct platform_model *model, void *ptr, size_t size){
	(void)model;
	(void)ptr;
	(void)size;	
}

void platform_log(struct platform_model *model, const char *message, struct text_attributes attr, uint8_t *psf){
	display_psf_string(model, psf, (char *)message, attr);
}

uint8_t *platform_read_file(struct platform_model *model, const char *file_name){
	boot_resources *resources = model->ctx;
	uint8_t *file_buffer;
	int file_name_len = string_len((char *)file_name);
	short unsigned int *file_wstr = (short unsigned int *)platform_alloc(model, file_name_len);
	string_to_wstring((char *)file_name, file_wstr);

	EFI_LOADED_IMAGE *loaded_image = NULL;
    EFI_GUID lip_guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_FILE_IO_INTERFACE *io_volume;
    EFI_GUID fs_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_FILE_HANDLE volume;

    resources->gBS->HandleProtocol(resources->image, &lip_guid, (void **)&loaded_image);
    resources->gBS->HandleProtocol(loaded_image->DeviceHandle, &fs_guid, (void *)&io_volume);
    io_volume->OpenVolume(io_volume, &volume);
    resources->root_volume = volume;

    EFI_FILE_HANDLE file;
    EFI_STATUS status;

    status = resources->root_volume->Open(resources->root_volume, &file, file_wstr, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);

    fail_if(status != 0, L"Failed to open file!\r\n", resources->gST);

    UINTN size = sizeof(EFI_FILE_INFO) + 100; /* TODO: dynamically resize buffer if fail */
    EFI_FILE_INFO *file_info;
    resources->gBS->AllocatePool(EfiLoaderCode, size, (void **)&file_info);
    UINTN file_size;
    EFI_GUID guid = EFI_FILE_INFO_ID;

    file->GetInfo(file, &guid, &size, (void *)file_info);
    file_size = file_info->FileSize;

	file_buffer = (uint8_t *)platform_alloc(model, file_size);
    file->Read(file, &file_size, file_buffer);

	return file_buffer;
}

void plot_pixel_32bpp(struct platform_model *model, int x, int y, uint32_t colour){
	boot_resources *resources = model->ctx;
	*((uint32_t *)(resources->fb->base + resources->fb->pitch * y + 4 * x)) = colour;
}

void init_platform(void *ctx, struct platform_model *model){
	model->alloc = platform_alloc;
	model->free = platform_free;
	model->log = platform_log;
	model->read_file = platform_read_file;
	model->plot32 = plot_pixel_32bpp;
	model->ctx = ctx;
}
