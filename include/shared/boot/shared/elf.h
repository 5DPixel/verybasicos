#ifndef BOOT_SHARED_ELF_H
#define BOOT_SHARED_ELF_H

#include <stdint.h>
#include <boot/shared/platform.h>
#include <elf.h>

int is_valid_elf(uint8_t *elf);
int check_elf_supported(uint8_t *elf);
int elf64_alloc_sections(struct platform_model *model, struct elf64_ehdr *hdr);
void *load_segment(struct platform_model *model, struct elf64_ehdr *hdr, struct elf64_phdr *phdr);
void load_segments(struct platform_model *model, struct elf64_ehdr *hdr);

#endif
