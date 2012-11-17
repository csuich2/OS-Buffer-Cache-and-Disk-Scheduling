#include <kernel.h>
#include <pa4.h>


/*
 * Part A 2/4. buf_close()
 * buf_close() deallocates all the resources that buffer manager has used.
 */
void buf_close(void) {
	STATWORD *ps;
	int found_invalid_block;
	disk_desc *d1, *d2;
	dsk_buffer_p dbp;

	disable(ps);

	buf_is_open = 0;

	// Wait until both disks have finished their I/O requests
	d1 = (disk_desc*)devtab[DISK0].dvioblk;
	d2 = (disk_desc*)devtab[DISK1].dvioblk;
	while ((d1 && d1->request_head) || (d2 && d2->request_head)) {
		restore(ps);
		sleep(1);
		disable(ps);
	}

	dbp = buf_head;
	do {
		found_invalid_block = 0;
		while (dbp != (dsk_buffer_p)NULL) {
			if (!dbp->valid) {
				found_invalid_block = 1;
				sleep(5);
				break;
			}
			dbp = dbp->next;
		}
	} while (found_invalid_block);

	dbp = buf_head;
	// Loop over all the blocks in the buffer cache
	while (dbp != (dsk_buffer_p)NULL) {
		// Save a reference to the next one
		dsk_buffer_p next = dbp->next;
		// If this block is dirty, write it to the disk
		if (dbp->dirty) {
			dskwrite(dbp->pdev, dbp->data, dbp->block_no, 1);
		}
		// Free the data
		freemem(dbp->data, 128);
		// Free the block
		freemem(dbp, sizeof(struct buf));
		// Move on to the next block
		dbp = next;
	}
	// Point the head to NULL
	buf_head = (dsk_buffer_p)NULL;
	// Reset the buf_count
	buf_count = 0;
}
