; initial kernel start

format ELF64

; in 64-bit mode, the base and limit values are ignored, the whole address space is used


; access byte format
; P | DPL | S | E | DC | RW | A
; P -- present bit (1 for any valid segment)
; DPL -- CPU privilege required (ring 0, ..., 2 bits!)
; S -- descriptor type (0 = system segment, e.g. a TSS, 1 = code/data segment)
; E -- executable bit (0 = data segment, 1 = code segment)
; DC -- direction bit (if 1, the segment grows down)
; RW -- for code segments, readable bit, for data segments, writeable bit
; A -- accessed bit, set by the CPU when accessing the segment (set to 1 in advance)

; flags format
; G -- granularity, if 1, the limit is in 4KiB blocks (pages)
; DB -- size flag, if 1, define a protected mode segment
; L -- long mode code flag, when set, DB should be clear
; reserved bit

kernel_code_segment_access_byte equ 0x9F
kernel_code_segment_flags equ 0xA0

kernel_data_segment_access_byte equ 0x97
kernel_data_segment_flags equ 0xA0 ; 0xA << 4

section '.bss'
stack_begin:
	rb 16384
stack_end:

section '.rodata'

gdt:
	.null: dq 0

	.code:
		dw 0 ; limit low
		times 3 db 0 ; 3 bytes for base
		db kernel_code_segment_access_byte
		db kernel_code_segment_flags
		db 0 ; base high
	
	.data:
		dw 0
		times 3 db 0
		db kernel_data_segment_access_byte
		db kernel_data_segment_flags
		db 0
	.end:

gdt_desc: dw gdt.end - gdt - 1

section '.text'

public kernel_start
extrn kernel_init

kernel_start:
	lgdt [gdt_desc]

	mov rsp, stack_end
	call kernel_init 
