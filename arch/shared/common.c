#include <common.h>

void *memcpy(void *restrict dest, const void *restrict src, size_t n){
    uint8_t *restrict pdest = (uint8_t *restrict)dest;
    const uint8_t *restrict psrc = (const uint8_t *restrict)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }

    return s;
}

bool power_of_two(size_t num){
	int i;
	int bit_count = 0;
	
	for(i = 0; i < (int)sizeof(num) * 8; i++){
		if(num & (0x80 >> i)){
			bit_count++;
			if(bit_count > 1) return false;
		}
	}
	
	return true;
}

int string_len(char *str){
    int i;
    for(i = 0; str[i] != '\0'; i++);

    return i;
}

int string_equals(char *a, char *b){
    int len = string_len(a) > string_len(b) ? string_len(a) : string_len(b);
    int i;

    for(i = 0; i < len; i++) if(a[i] != b[i]) return -1;

    return 0;
}

size_t align_size_forward(size_t size, size_t align){
    return CEIL_DIV(size, align) * align;
}

uint32_t align_size_power_of_two(uint32_t size){
	size--;
	size |= size >> 1;
	size |= size >> 2;
	size |= size >> 4;
	size |= size >> 8;
	size |= size >> 16;
	size++;
	
	return size;
}
