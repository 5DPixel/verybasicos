OUTPUT_FORMAT(elf64-x86-64)

ENTRY(kernel_start)

/* program header permissions to exert more control over linking process */
PHDRS
{
	text PT_LOAD;
	rodata PT_LOAD;
	data PT_LOAD;
}

SECTIONS
{
	/*. = 0xffffffff80000000; /* TODO: uncomment and set up page tables for higher half */
	. = 1M;
	
	.text : {
		*(.text .text.*)
	} : text
	
	. = ALIGN(CONSTANT(MAXPAGESIZE));

	.rodata : {
		*(.rodata .rodata.*)
	} : rodata

	.note.gnu.build-id : {
		*(.note.gnu.build-id)
	} : note.gnu-build-id

	. = ALIGN(CONSTANT(MAXPAGESIZE));

	.data : {
		*(.data .data.*)
	} : data

	.bss : {
		*(.bss .bss.*)
		*(COMMON)
	}

	. = ALIGN(CONSTANT(MAXPAGESIZE));
	_kernel_heap_start = .;

	/DISCARD/ : {
		*(.eh_frame*)
		*(.note .note.*)
	}
}
