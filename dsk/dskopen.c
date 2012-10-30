#include <kernel.h>
#include <pa4.h>

extern int strcpy(char *dest, char *src);

/*
 * dskopen() does
 *     if the corresponding profile data have been set and a virtual disk has been created,
 *         just return OK.
 *     otherwise
 *         return SYSERR.
 * parameters:
 *     pdev:	device descriptor
 */
int dskopen(struct devsw *pdev) {
	disk_desc *ptr;

	if(!pdev -> dvioblk) 
		return SYSERR;
	else
		return OK;

	return OK;
}
