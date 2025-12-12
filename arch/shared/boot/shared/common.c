#include <common.h>
#include <boot/shared/common.h>

void int_to_wstring(int n, short unsigned int *str){
    int i = 0, j = 0, k = 0;
    int sign = n;

    if (n < 0)
        n = -n;

    while(n > 0){
        str[i++] = n % 10 + '0';
        n /= 10;
    }

    if(sign < 0)
        str[i++] = '-';

    str[i] = '\r';
    str[i + 1] = '\n';
    str[i + 2] = '\0';

    for(j = 0, k = i; j < k; j++, k--){
        XOR_SWAP(str[j], str[k]);
    }
}

int wstring_len(short unsigned int *str){
    int i;
    for(i = 0; str[i] != '\0'; i++);

    return i;
}

int string_len(char *str){
    int i;
    for(i = 0; str[i] != '\0'; i++);

    return i;
}

void wstringle_to_string(char *str, short unsigned int *wstr){
    int i;

    for(i = 0; wstr[i] != '\0'; i++)
        str[i] = (unsigned char)wstr[i] & 0xFF;
}

void string_to_wstringle(char *str, short unsigned int *wstr){
    int i;

    for(i = 0; str[i] != '\0'; i++)
        wstr[i] = str[i];
}

void wstring_to_string(char *str, short unsigned int *wstr){
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    wstringle_to_string(str, wstr);
    #else
    #error "Big endian systems aren't yet supported!"
    #endif
}

void string_to_wstring(char *str, short unsigned int *wstr){
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    string_to_wstringle(str, wstr);
    #else
    #error "Big endian systems aren't yet supported!"
    #endif
}


void *memcpy(void *restrict dest, const void *restrict src, size_t n){
    uint8_t *restrict pdest = (uint8_t *restrict)dest;
    const uint8_t *restrict psrc = (const uint8_t *restrict)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

int string_equals(char *a, char *b){
	int len = string_len(a);
	int i;
	
	for(i = 0; i < len; i++) if(a[i] != b[i]) return -1;

	return 0;
}

void *memset(void *s, int c, size_t n) {
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }

    return s;
}
