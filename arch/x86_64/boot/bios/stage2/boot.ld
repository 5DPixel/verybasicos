OUTPUT_FORMAT(elf64-x86-64)

ENTRY(boot_stage2_start)

PHDRS
{
	text PT_LOAD;
	rodata PT_LOAD;
	data PT_LOAD;
}

SECTIONS
{
	. = 1M; /* can be replaced by kernel later, doesn't matter */
	
	.text : {
		*(.text .text.*)
	} : text
	
	. = ALIGN(CONSTANT(MAXPAGESIZE));
	
	.rodata : {
		*(.rodata .rodata.*)
	} : rodata
	
	.note.gnu.build-id : {
		*(.note.gnu.build-id)
	} : note.gnu.build-id
	
	. = ALIGN(CONSTANT(MAXPAGESIZE));
	
	.data : {
		*(.data .data.*)
	} : data
	
	.bss : {
		*(.bss .bss.*)
		*(COMMON)
	}
	
	. = ALIGN(CONSTANT(MAXPAGESIZE));
	_heap_start = .;
	
	/DISCARD/ : {
		*(.eh_frame*)
		*(.note .note.*)
	}
}

