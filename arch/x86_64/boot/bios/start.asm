use16

org 0x7c00

; as per my small platform "spec", we should be able to allocate arbitrary memory and pages, free memory, get the memory map, framebuffer, initialise the platform, get a key, get the platform type, and choose a framebuffer mode

; get into long mode (by reusing some code), use V86 tasks in C with assembly stubs
; to query long mode, we need to be in protected mode

macro bios_write str
{
	mov si, str
	call print
}

boot_label:
	jmp 0x000:start
	times 8 - ($ - $$) db 0
	
	bi_primary_volume_descriptor rd 1
	bi_boot_file_location rd 1
	bi_boot_file_length rd 1
	bi_checksum rd 1
	bi_reserved rd 40

sectors_per_track dw 9
heads dw 2

boot_drive rb 1
disk_data_seg = 0x100

; TODO: is the boot number correct?

start:
	cli
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	jmp switch_to_protected_mode
	
	jmp $

; string in ds:si
print:
	mov ah, 0x0e
	lodsb
	int 0x10
	
	cmp al, 0
	jne print
	ret

kernel_code_segment_access_byte equ 0x9A
kernel_code_segment_flags equ 0xCF

kernel_data_segment_access_byte equ 0x92
kernel_data_segment_flags equ 0xCF ; 0xE << 4

kernel_code_segment_limit_and_flags equ 0xFE
kernel_data_segment_limit_and_flags equ 0xFE

; TODO: 64-bit GDT ignores limits, the 32-bit one doesn't, that's why the code won't work!

gdt_32:
    .null: dq 0

    .code:
        dw 0xFFFF ; limit low
        times 3 db 0 ; 3 bytes for base
        db kernel_code_segment_access_byte
        db kernel_code_segment_limit_and_flags
        db 0 ; base high

    .data:
        dw 0xFFFF
        times 3 db 0
        db kernel_data_segment_access_byte
        db kernel_data_segment_limit_and_flags
        db 0
    .end:

gdt_32_desc: dw gdt_32.end - gdt_32 - 1
gdt: dd gdt_32 ; required for protected mode
gdt_32_code = gdt_32.code - gdt_32

switch_to_protected_mode:
	in al, 0x92 ; fast A20 line
	or al, 2
	out 0x92, al
	
	lgdt [gdt_32_desc]
	
	mov eax, cr0
	or al, 1 ; set PE
	mov cr0, eax
	
	mov esp, stack_end
	
	jmp gdt_32_code:protected_main
	hlt

protected_main:
	hlt

stack_begin:
	rb 4096
stack_end:
