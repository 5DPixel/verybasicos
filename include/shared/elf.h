#ifndef SHARED_ELF_H
#define SHARED_ELF_H

#include <stdint.h>
#include <stddef.h>

typedef uint64_t elf64_addr;
typedef uint64_t elf64_xword;
typedef uint64_t elf64_sxword;
typedef uint64_t elf64_off;

typedef int32_t elf64_sword;
typedef uint32_t elf64_word;

typedef uint16_t elf64_half;

/* file types */
#define ET_NONE 0
#define ET_REL 1
#define ET_EXEC 2
#define ET_DYN 3
#define ET_CORE 4

/* architectures */
#define EM_386 3
#define EM_X86_64 62

/* e ident */
#define EI_MAG0 0
#define EI_MAG1 1
#define EI_MAG2 2
#define EI_MAG3 3
#define EI_CLASS 4
#define EI_DATA 5
#define EI_VERSION 6
#define EI_OSABI 7
#define EI_ABIVERSION 8
#define EI_PAD 9

#define EI_NIDENT 16

#define SHN_UNDEF 0
#define SHN_ABS 0xFFF1

/* ELF header */
struct elf64_ehdr {
	unsigned char e_ident[EI_NIDENT];
	elf64_half e_type;
	elf64_half e_machine;
	elf64_word e_version;
	elf64_addr e_entry;
	elf64_off e_phoff; /* program header offset */
	elf64_off e_shoff;
	elf64_word e_flags;
	elf64_half e_ehsize;
	elf64_half e_phentsize;
	elf64_half e_phnum;
	elf64_half e_shentsize;
	elf64_half e_shnum;
	elf64_half e_shstrndx;
};

/* section header */
struct elf64_shdr {
	elf64_word sh_name;
	elf64_word sh_type;
	elf64_xword sh_flags;
	elf64_addr sh_addr;
	elf64_off sh_offset;
	elf64_xword sh_size;
	elf64_word sh_link;
	elf64_word sh_info;
	elf64_xword sh_addralign;
	elf64_xword sh_entsize;
};

struct elf64_sym {
	elf64_word st_name;
	unsigned char st_info;
	unsigned char st_other;
	elf64_half st_shndx;
	elf64_addr st_value;
	elf64_xword st_size;
};

struct elf64_phdr {
	elf64_word p_type;
	elf64_word p_flags;
	elf64_off p_offset;
	elf64_addr p_vaddr;
	elf64_addr p_paddr;
	elf64_xword p_filesz;
	elf64_xword p_memsz;
	elf64_xword p_align;
};

enum sht_types {
	SHT_NULL = 0,
	SHT_PROGBITS = 1,
	SHT_SYMTAB = 2,
	SHT_STRTAB = 3,
	SHT_RELA = 4,
	SHT_HASH = 5,
	SHT_DYNAMIC = 6,
	SHT_NOTE = 7,
	SHT_NOBITS = 8,
	SHT_REL = 9
};

enum sht_attributes {
	SHF_WRITE = 0x01,
	SHF_ALLOC = 0x02
};

enum stt_bindings {
	STT_NOTYPE = 0,
	STT_OBJECT = 1,
	STT_FUNC = 2,
	STT_SECTION = 3
};

enum stb_bindings {
	STB_LOCAL = 0,
	STB_GLOBAL = 1,
	STB_WEAK = 2
};

enum pt_type {
	PT_NULL = 0,
	PT_LOAD = 1
};

static inline struct elf64_shdr *elf64_sheader(struct elf64_ehdr *hdr){
	return (struct elf64_shdr *)((char *)hdr + hdr->e_shoff);
}

static inline struct elf64_phdr *elf64_pheader(struct elf64_ehdr *hdr){
	return (struct elf64_phdr *)((char *)hdr + hdr->e_phoff);
}

static inline struct elf64_shdr *elf64_section(struct elf64_ehdr *hdr, int index){
	return &elf64_sheader(hdr)[index];
}

static inline struct elf64_phdr *elf64_program_header(struct elf64_ehdr *hdr, int index){
	return &elf64_pheader(hdr)[index];
}

static inline char *elf64_str_table(struct elf64_ehdr *hdr) {
	if(hdr->e_shstrndx == SHN_UNDEF) return NULL;
	return (char *)hdr + elf64_section(hdr, hdr->e_shstrndx)->sh_offset;
}

static inline char *elf64_string(struct elf64_ehdr *hdr, int offset) {
	char *strtab = elf64_str_table(hdr);
	if(strtab == NULL) return NULL;
	return strtab + offset;
}

#endif
