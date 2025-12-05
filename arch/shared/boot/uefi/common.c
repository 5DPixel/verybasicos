#include <common.h>
#include <boot/common.h>

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

void fail_if(int condition, CHAR16 *wstr, EFI_SYSTEM_TABLE *gST){
    if(!condition) return;

    gST->ConOut->OutputString(gST->ConOut, wstr);

    goto cleanup;

	cleanup:
    	return;
}

void shutdown_system(boot_resources *resources){
	resources->gRT->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
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
