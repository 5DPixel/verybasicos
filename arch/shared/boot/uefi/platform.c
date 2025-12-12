#include <boot/shared/platform.h>
#include <boot/uefi/common.h>
#include <boot/shared/psf.h>
#include <common.h>
#include <efi.h>
#include <kernel/const.h>

void *platform_alloc(struct platform_model *model, size_t size){
	// expect ctx to be the boot_resources struct
	void *buffer;
	struct uefi_boot_resources *resources = model->ctx;
	resources->gBS->AllocatePool(EfiLoaderCode, (UINTN)size, (void **)&buffer);
	
	return buffer;
}

void platform_alloc_pages(struct platform_model *model, int page_count, uint64_t addr){	
	EFI_PHYSICAL_ADDRESS *paddr = (EFI_PHYSICAL_ADDRESS *)&addr;
	struct uefi_boot_resources *resources = model->ctx;
	resources->gBS->AllocatePages(AllocateAddress, EfiLoaderCode, page_count, paddr);
}

void platform_free(struct platform_model *model, void *ptr){	
	struct uefi_boot_resources *resources = model->ctx;
	resources->gBS->FreePool(ptr);
}

void platform_write(struct platform_model *model, const char *message, struct text_attributes *attr){
	display_psf_string(model, model->font, (char *)message, attr);
}

uint8_t *platform_read_file(struct platform_model *model, const char *file_name){
	struct uefi_boot_resources *resources = model->ctx;
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
	
	file->Close(file);

	return file_buffer;
}

struct mmap_entry *platform_get_mmap_entries(struct platform_model *model, int *mmap_entry_count){
	struct uefi_boot_resources *resources = model->ctx;
	int i;

	EFI_MEMORY_DESCRIPTOR *mmap = NULL;
	UINTN mmap_size; 
	UINTN map_key;
	UINTN descriptor_size;
	UINT32 descriptor_version;

	resources->gBS->GetMemoryMap(&mmap_size, mmap, &map_key, &descriptor_size, &descriptor_version);
	
	*mmap_entry_count = (mmap_size / descriptor_size);

	struct mmap_entry *entries = model->alloc(model, (mmap_size / descriptor_size) * sizeof(struct mmap_entry));

	mmap = model->alloc(model, mmap_size);
	resources->gBS->GetMemoryMap(&mmap_size, mmap, &map_key, &descriptor_size, &descriptor_version);
	
	resources->mmap_key = map_key;

	for(i = 0; i < (int)(mmap_size / descriptor_size); i++){
		EFI_MEMORY_DESCRIPTOR *descriptor = (EFI_MEMORY_DESCRIPTOR *)((char *)mmap + i * descriptor_size);
		entries[i].phys_base = descriptor->PhysicalStart;
		entries[i].virt_base = descriptor->VirtualStart;
		entries[i].size = EFI_PAGE_SIZE * descriptor->NumberOfPages;
		
		switch(descriptor->Type){
			case EfiReservedMemoryType: {
				entries[i].type = MEM_RESERVED;
				break;
			}
			
			case EfiConventionalMemory: {
				entries[i].type = MEM_FREE;
				break;
			}

			case EfiUnusableMemory: {
				entries[i].type = MEM_BAD;
				break;
			}
			
			case EfiACPIReclaimMemory: {
				entries[i].type = MEM_ACPI;
				break;
			}
			
			case EfiACPIMemoryNVS: {
				entries[i].type = MEM_RESERVED;
				break;
			}
		}
	}

	return entries;
}

void platform_exit(struct platform_model *model){
	struct uefi_boot_resources *resources = model->ctx;
	
	resources->gBS->ExitBootServices(resources->image, resources->mmap_key);
	/* platform can no longer be used */
}

struct framebuffer *platform_display_attributes(struct platform_model *model){
	struct uefi_boot_resources *resources = model->ctx;
	
	return resources->fb;
}

enum platform_type platform(struct platform_model *model){
	(void)model;

	return PLATFORM_UEFI; /* super simple function, if user is compiling this file, it's UEFI */
}

enum key get_key(struct platform_model *model){
	struct uefi_boot_resources *resources = model->ctx;
	EFI_INPUT_KEY key;
	while(resources->gST->ConIn->ReadKeyStroke(resources->gST->ConIn, &key));
	
	if(key.UnicodeChar == L'\r') return KEY_SPACE;

	return (enum key)key.ScanCode;
}

void plot_pixel_32bpp(struct platform_model *model, int x, int y, uint32_t colour){
	struct uefi_boot_resources *resources = model->ctx;
	*((uint32_t *)(resources->fb->base + resources->fb->pitch * y + 4 * x)) = colour;
}

void init_platform(void *ctx, const char *font_file_path, const char *kernel_file_path, struct platform_model *model){
	model->alloc = platform_alloc;
	model->alloc_pages = platform_alloc_pages;
	model->free = platform_free;
	model->write = platform_write;
	model->read_file = platform_read_file;
	model->plot32 = plot_pixel_32bpp;
	model->display_attributes = platform_display_attributes;
	model->mmap_entries = platform_get_mmap_entries;
	model->exit = platform_exit;
	model->get_key = get_key;
	model->platform = platform;
	model->ctx = ctx;
	model->log = log;
	model->font_dimensions = font_dimensions;
	model->clear_screen = clear_screen;
	model->internal_log_buffer = (struct log_circular_buffer *)model->alloc(model, sizeof(struct log_circular_buffer));
	model->font = model->read_file(model, font_file_path);
	model->kernel = model->read_file(model, kernel_file_path);

    struct framebuffer *fb = model->display_attributes(model);
    
	model->text_attr = (struct text_attributes *)model->alloc(model, sizeof(struct text_attributes));
    model->text_attr->x = fb->width / 16;
    model->text_attr->y = fb->height / 16;
    model->text_attr->fg = PLATFORM_WHITE;
    model->text_attr->bg = PLATFORM_BLACK;

	log_cb_init(model, model->internal_log_buffer, MAX_BUFFER_LOGS);
}

void log(struct platform_model *model, char *message, enum log_severity severity){
    struct log_record *log = create_log(model, message, severity);
    log_cb_push_back(model, model->internal_log_buffer, log);
    log_cb_entry(model, model->internal_log_buffer, model->text_attr);
}

void font_dimensions(struct platform_model *model, int *width, int *height){
	*width = font_width(model->font);
	*height = font_width(model->font);
}

void clear_screen(struct platform_model *model, uint32_t colour){
	struct framebuffer *fb = platform_display_attributes(model);
	int i;
	for(i = 0; i < (int)(fb->width * fb->height); i++)
		model->plot32(model, (i % fb->width), (i / fb->width), colour);
}
