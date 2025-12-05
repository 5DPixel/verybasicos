#ifndef _ELF_H
#define _ELF_H

#include <stdint.h>

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

typedef struct {
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
} elf64_ehdr;

int test();

#endif
