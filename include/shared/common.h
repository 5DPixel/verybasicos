#ifndef SHARED_COMMON_H
#define SHARED_COMMON_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define XOR_SWAP(a, b) do { \
	a ^= b; \
	b ^= a; \
	a ^= b; \
} while(0)

#define CEIL_DIV(x,y) (((x) + (y) - 1) / (y))

#define COLOUR_RGB32(r, g, b) (uint32_t)(r << 16) | (uint32_t)(g << 8) | (uint32_t)(b << 0)
#define COLOUR_BGR32(b, g, r) (uint32_t)(r << 16) | (uint32_t)(g << 8) | (uint32_t)(b << 0)

struct framebuffer {
	uint64_t base;
	uint64_t size;
	uint8_t bpp;
	uint32_t pitch;
	uint32_t width;
	uint32_t height;

	uint8_t red_mask;
	uint8_t green_mask;
	uint8_t blue_mask;
};

void int_to_wstring(int n, short unsigned int *str);
int wstring_len(short unsigned int *str);
int string_len(char *str);
void wstringle_to_string(char *str, short unsigned int *wstr);
void string_to_wstringle(char *str, short unsigned int *wstr);
void wstring_to_string(char *str, short unsigned int *wstr);
void string_to_wstring(char *str, short unsigned int *wstr);

void halt_system();

void *memcpy(void *restrict dest, const void *restrict src, size_t n);
void *memset(void *s, int c, size_t n);

int string_equals(char *a, char *b);
bool power_of_two(size_t num);

size_t align_size_forward(size_t size, size_t align);
uint32_t align_size_power_of_two(uint32_t size);

#endif
