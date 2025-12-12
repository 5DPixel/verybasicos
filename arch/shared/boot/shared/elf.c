#include <boot/shared/elf.h>
#include <kernel/const.h>
#include <elf.h>
#include <common.h>

int is_valid_elf(uint8_t *elf){
	struct elf64_ehdr *header = (struct elf64_ehdr *)elf; //only works for LE systems!
	
	if(header->e_ident[EI_MAG0] != 0x7F) return -1;
	if(header->e_ident[EI_MAG1] != 'E') return -1;
	if(header->e_ident[EI_MAG2] != 'L') return -1;
	if(header->e_ident[EI_MAG3] != 'F') return -1;

	return 0;
}

int check_elf_supported(uint8_t *elf){
	struct elf64_ehdr *header = (struct elf64_ehdr *)elf;

	if(header->e_machine != EM_X86_64) return -1;

	return 0;
}

int elf64_alloc_sections(struct platform_model *model, struct elf64_ehdr *hdr){
	int i;
	struct elf64_shdr *shdr = elf64_sheader(hdr);

	for(i = 0; i < hdr->e_shnum; i++){
		struct elf64_shdr *section = &shdr[i];

		if(section->sh_type != SHT_NOBITS || !section->sh_size || !(section->sh_flags & SHF_ALLOC)) continue;
		
		void *mem = model->alloc(model, section->sh_size);
		if(mem == NULL) return -1;
		memset(mem, 0, section->sh_size);
		
		section->sh_offset = (uint64_t)((char *)mem - (char *)hdr);
	}

	return 0;
}

void *load_segment(struct platform_model *model, struct elf64_ehdr *hdr, struct elf64_phdr *phdr){
	if(phdr->p_type != PT_LOAD) return NULL;

	int segment_page_count = CEIL_DIV(phdr->p_memsz, PAGE_SIZE);
	model->alloc_pages(model, segment_page_count, phdr->p_paddr);
	uint8_t *pages = (uint8_t *)phdr->p_paddr;
	uint8_t *segment = ((uint8_t *)hdr + phdr->p_offset);	
	
	memcpy(pages, segment, phdr->p_filesz);

	/* as per the elf spec, zero extra memory in a segment */

	if(phdr->p_memsz > phdr->p_filesz){
		uint8_t *zero_memory = ((uint8_t *)segment + phdr->p_memsz - phdr->p_filesz);
		memset((void *)zero_memory, 0, phdr->p_memsz - phdr->p_filesz);
	}

	return pages;
}

void load_segments(struct platform_model *model, struct elf64_ehdr *hdr){
	struct elf64_phdr *phdrs = elf64_pheader(hdr);
	int i;
	
	for(i = 0; i < hdr->e_phnum; i++){
		if(phdrs[i].p_type != PT_LOAD) continue;
		
		load_segment(model, hdr, &phdrs[i]);
	}
}
