#include <boot/uefi/file.h>
#include <boot/uefi/common.h>

#ifdef __GNUC__
#include <assert.h>
#endif

void get_root_volume(boot_resources *resources){
	EFI_LOADED_IMAGE *loaded_image = NULL;
	EFI_GUID lip_guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
	EFI_FILE_IO_INTERFACE *io_volume;
	EFI_GUID fs_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
	EFI_FILE_HANDLE volume;
	
	//#ifdef __GNUC__
	//static_assert(resources->image == NULL, "Resource image handle was NULL!"); /* gcc specific asserts, TODO: compatibility */
	//#endif
	
	resources->gBS->HandleProtocol(resources->image, &lip_guid, (void **)&loaded_image);
	resources->gBS->HandleProtocol(loaded_image->DeviceHandle, &fs_guid, (void *)&io_volume);
	io_volume->OpenVolume(io_volume, &volume);
	resources->root_volume = volume;
}

/* handles == pointer types */
void open_kernel_file(boot_resources *resources, CHAR16 *file_name){
	EFI_FILE_HANDLE file_handle;
	EFI_STATUS status;

	status = resources->root_volume->Open(resources->root_volume, &file_handle, file_name, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);

	fail_if(status != 0, L"Failed to open file!\r\n", resources->gST);
	resources->kernel_file = file_handle;
	resources->kernel_file_length = file_size(resources, resources->kernel_file);
}

void read_kernel_file(boot_resources *resources){
	resources->gBS->AllocatePool(EfiLoaderCode, resources->kernel_file_length, (void **)&resources->kernel_file_buffer);
	resources->kernel_file->Read(resources->kernel_file, &resources->kernel_file_length, resources->kernel_file_buffer);
}

UINT64 file_size(boot_resources *resources, EFI_FILE_HANDLE file){
    UINTN size = sizeof(EFI_FILE_INFO) + 100; /* TODO: dynamically resize buffer if fail */
    EFI_FILE_INFO *file_info;
	resources->gBS->AllocatePool(EfiLoaderCode, size, (void **)&file_info);
	UINTN file_size;
	EFI_GUID guid = EFI_FILE_INFO_ID;

	file->GetInfo(file, &guid, &size, (void *)file_info);
	file_size = file_info->FileSize;

	return file_size;
}
