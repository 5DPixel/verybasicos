#include <boot/shared/boot.h>
#include <boot/shared/elf.h>
#include <kernel.h>
/* where the magic happens! */

void start_default_boot(struct platform_model *model){
	model->clear_screen(model, PLATFORM_BLACK);

	struct elf64_ehdr *hdr = (struct elf64_ehdr *)model->kernel;
	uint64_t entry = hdr->e_entry;

	model->log(model, "Starting default boot...\n", LOG_SEVERITY_NONE);
	if(is_valid_elf(model->kernel) == -1){
		model->log(model, "Kernel isn't a valid ELF file!\n", LOG_SEVERITY_ERROR);
		return;
	}

	if(check_elf_supported(model->kernel) == -1){
		model->log(model, "Kernel system architecture doesn't match this machine!\n", LOG_SEVERITY_ERROR);
		return;
	}

	if(elf64_alloc_sections(model, hdr) == -1){
		model->log(model, "Couldn't allocate memory for section!\n", LOG_SEVERITY_ERROR);
		
		return;
	}

	model->log(model, "Allocated memory for empty section(s)\n", LOG_SEVERITY_NONE);

	model->log(model, "Retrieved system memory map\n", LOG_SEVERITY_NONE);
	
	struct kernel_boot_params *params = model->alloc_conventional(model, sizeof(struct kernel_boot_params));
	params->fb = model->alloc_conventional(model, sizeof(struct framebuffer));
	params->font = model->alloc_conventional(model, model->font_length);

	model->log(model, "Past statement...\n", LOG_SEVERITY_NONE);
	memcpy(params->fb, model->display_attributes(model), sizeof(struct framebuffer));
	memcpy(params->font, model->font, model->font_length);

	model->log(model, "Loading kernel...\n", LOG_SEVERITY_NONE);

	load_segments(model, hdr);

	model->log(model, "Exiting to kernel...\n", LOG_SEVERITY_NONE);
	log_cb_free(model, model->internal_log_buffer);

	model->free(model, model->font);
	model->free(model, model->kernel);

	int mmap_entry_count;
	struct mmap_entry *entries = platform_get_mmap_entries(model, &mmap_entry_count);
	params->mem_regions = entries;
	params->mem_regions = model->alloc_conventional(model, mmap_entry_count * sizeof(struct mmap_entry));	
	model->exit(model);
	
	/* to the kernel! */
	void (*kernel_init)(struct kernel_boot_params *params) = (void *)entry;
	kernel_init(params);
}
