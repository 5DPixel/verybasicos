#include <boot/shared/boot.h>
#include <boot/shared/elf.h>
#include <kernel.h>
/* where the magic happens! */

void start_default_boot(struct platform_model *model){
	model->clear_screen(model, PLATFORM_BLACK);

	struct elf64_ehdr *hdr = (struct elf64_ehdr *)model->kernel;

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

	int mmap_entry_count;
	struct mmap_entry *entries = platform_get_mmap_entries(model, &mmap_entry_count);

	model->log(model, "Retrieved system memory map\n", LOG_SEVERITY_NONE);
	
	struct kernel_boot_params *params = model->alloc(model, sizeof(struct kernel_boot_params));
	params->fb = model->display_attributes(model);
	params->mem_region_count = mmap_entry_count;
	params->font = model->font;	
	memset(params->mem_regions, 0, MAX_MMAP_ENTRY_COUNT * sizeof(struct mmap_entry));
	memcpy(params->mem_regions, entries, mmap_entry_count * sizeof(struct mmap_entry));

	model->log(model, "Loading kernel...\n", LOG_SEVERITY_NONE);

	load_segments(model, hdr);
	log_cb_free(model, model->internal_log_buffer);
	
	model->clear_screen(model, PLATFORM_BLACK);
	model->free(model, model->font); /* TODO: debug why freeing model->kernel results in framebuffer text not rendering*/
	model->exit(model);
	
	/* to the kernel! */
	void (*kernel_main)(struct kernel_boot_params *params) = (void *)hdr->e_entry;
	kernel_main(params);
}
