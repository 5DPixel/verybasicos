use16

org 0x7c00

boot_label:
	jmp 0x000:start
	times 8 - ($ - $$) db 0
	
	bi_primary_volume_descriptor rd 1
	bi_boot_file_location rd 1
	bi_boot_file_length rd 1
	bi_checksum rd 1
	bi_reserved rd 40

start:
	cli
	;xor ax, ax
	;mov ds, ax
	;mov es, ax
	;mov fs, ax
	;mov gs, ax
	
	;mov ah, 0x0e
	;mov al, 'A'
	;int 0x10
	mov si, boot_string
	call print
	hlt

boot_string db "booting verybasicos...", 0

; string in ds:si
print:
	mov ah, 0x0e
	lodsb
	int 0x10
	
	cmp al, 0
	jne print
	ret


;mov si, boot_string
;call print

times 512 - ($ - $$) db 0
dw 0xAA55
