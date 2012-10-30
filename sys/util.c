#include <kernel.h>

/* 
 * These functions aim at copying memory to memory or string to string.
 * Parameters:
 *     dest:	the destination of memory copy
 *     src: 	the source of memory copy
 *     num:	the number of bytes
 */
int memcpy(void *dest, void *src) {
	STATWORD ps;

	disable(ps);
	if(!dest || !src) {
		restore(ps);
		return SYSERR;
	}

	while(*(unsigned char *)src != '\0')
		*((unsigned char *)dest++) = *((unsigned char *)src++);
	
	restore(ps);
	return OK;
}

int memncpy(void *dest, void *src, int num) {
	STATWORD ps;

	disable(ps);
	if(!dest || !src || num < 0) {
		restore(ps);
		return SYSERR;
	}

	while(num-- > 0) 
		*((unsigned char *)dest++) = *((unsigned char *)src++);

	restore(ps);
	return OK;
}

int strcpy(char *dest, char *src) {
	STATWORD ps;

	disable(ps);
	if(!dest || !src) {
		restore(ps);
		return SYSERR;
	}

	while(*src != '\0')
		*dest++ = *src++;
	*dest = '\0';

	restore(ps);
	return OK;
}
