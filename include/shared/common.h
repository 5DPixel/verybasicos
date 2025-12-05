#ifndef _SHARED_COMMON_H
#define _SHARED_COMMON_H

#define XOR_SWAP(a, b) do { \
	a ^= b; \
	b ^= a; \
	a ^= b; \
} while(0)

void int_to_wstring(int n, short unsigned int *str);
int wstring_len(short unsigned int *str);
void wstringle_to_string(char *str, short unsigned int *wstr);
void string_to_wstringle(char *str, short unsigned int *wstr);

#endif
