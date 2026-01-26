OUTPUT_FORMAT(binary)

ENTRY(bios_main)

SECTIONS
{
	. = 0x7E00;
	
	.text : {
		*(.text .text.*)
	}
	
	. = ALIGN(CONSTANT(MAXPAGESIZE));
	
	.rodata : {
		*(.rodata .rodata.*)
	}
	
	. = ALIGN(CONSTANT(MAXPAGESIZE));
	
	.data : {
		*(.data .data.*)
	}
	
	.bss : {
		*(.bss .bss.*)
		*(COMMON)
	}
	
	. = ALIGN(CONSTANT(MAXPAGESIZE));
	
	_heap_start = .;
}
