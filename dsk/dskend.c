#include <kernel.h>
#include <pa4.h>

/*
 * dskend() releases all resources used by each device descriptor.
 * parameters:
 *     pdev:	device descriptor
 */
int dskend(struct devsw *pdev) {
	disk_desc *ptr;

	ptr = (disk_desc *)pdev -> dvioblk;

	if(ptr) {
		if(ptr -> disk)
			freemem((struct mblock *)ptr -> disk, ptr -> logical_blocks * ptr -> block_size);
		if(ptr -> seeks)
			freemem((struct mblock *)ptr -> seeks, ptr -> number_of_seeks * sizeof(seek_profile));

		freemem((struct mblock *)ptr, sizeof(disk_desc));
	}
	pdev -> dvioblk = NULL;
	return OK;
}

