; initial kernel start

format ELF64

section '.bss'
stack_begin:
	rb 16384
stack_end:

section '.text'

public kernel_start
extrn kernel_init

kernel_start:
	mov rsp, stack_end
	call kernel_init 
