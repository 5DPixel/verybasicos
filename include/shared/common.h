#ifndef _SHARED_COMMON_H
#define _SHARED_COMMON_H

#include <stdint.h>

#define XOR_SWAP(a, b) do { \
	a ^= b; \
	b ^= a; \
	a ^= b; \
} while(0)

typedef struct {
	uint64_t base;
	uint64_t size;
	uint8_t bpp;
	uint32_t pitch;
	uint32_t width;
	uint32_t height;

	uint8_t red_mask;
	uint8_t green_mask;
	uint8_t blue_mask;
} framebuffer;

void int_to_wstring(int n, short unsigned int *str);
int wstring_len(short unsigned int *str);
int string_len(char *str);
void wstringle_to_string(char *str, short unsigned int *wstr);
void string_to_wstringle(char *str, short unsigned int *wstr);
void wstring_to_string(char *str, short unsigned int *wstr);
void string_to_wstring(char *str, short unsigned int *wstr);

#endif
