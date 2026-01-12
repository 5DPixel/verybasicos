void boot_main(){
	volatile uint8_t *video_mem = (volatile uint8_t *)0xB8000;
	
	video_mem[0] = 'A'; /* TODO: call C from assembly by loading at 0x7E00 and add a bootloader signature */
	video_mem[1] = 0xF;
}
