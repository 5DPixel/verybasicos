/* TODO: use C file */
/* undefined reference */

#include <stdint.h>
#include <elf.h>

void bios_main(){
	__asm__("hlt");
	volatile uint8_t *video_mem = (volatile uint8_t *)0xB8000;
	
	video_mem[0] = 'A';
	video_mem[1] = 0xF;
}
