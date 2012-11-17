#include <kernel.h>
#include <pa4.h>

static void print_stat(disk_desc *ptr);

/*
 * dskclose() just prints statistics gathered from dskopen().
 * parameters:
 *     pdev:	device descriptor
 */
int dskclose(struct devsw *pdev) {
	disk_desc *ptr;
	dsk_buffer_p previous, current;
	STATWORD *ps;

	disable(ps);

	current = buf_head;
	previous = (dsk_buffer_p)NULL;
	// Traverse the list of buffer cache blocks
	while (current != (dsk_buffer_p)NULL) {
		// If we find one for this disk
		if (current->pdev == pdev) {
			// Save a reference to the next one
			dsk_buffer_p next = current->next;
			// If this is the first block, point the head
			// to the next block
			if (previous == (dsk_buffer_p)NULL) {
				buf_head = next;
			// Otherwise point previous to the next block
			} else {
				previous->next = next;
			}
			// Release the data member
			freemem(current->data, 128);
			// Release the buffer cache block itself
			freemem(current, sizeof(struct buf));
			// Update the buffer block count
			buf_count--;
			// Move on to the next block
			current = next;
			// previous stays the same
		// Otherwise move on to the next block
		} else {
			previous = current;
			current = current->next;
		}
	}

	restore(ps);

	ptr = (disk_desc *)pdev -> dvioblk;

	if(ptr) {
		print_stat(ptr);
		ptr -> seek_time = 0.0;
		ptr -> rotate_time = 0.0;
		ptr -> transfer_time = 0.0;
		ptr -> no_of_reads = 0;
		ptr -> no_of_writes = 0;
	}
	return OK;
}


static void print_stat(disk_desc *ptr) {
	STATWORD ps;

	disable(ps);
	if(!ptr) {
		restore(ps);
		return;
	}
	printf("\n\n============================================================================\n\n");
	printf("\tVendor: %s\tID: %s\tRevision: %s\n", ptr -> vendor, ptr -> id, ptr -> revision);
	printf("\tLogical Blocks: %d\n", ptr -> logical_blocks);
	printf("\tBlock Size: %d\n", ptr -> block_size);
	printf("\tOuter Track Size: %d\n", ptr -> outer_track_size);
	printf("\tSpeed: ");
	print_double(ptr -> speed);
	printf("\n\n\tNumber of Reads: %d\n", ptr -> no_of_reads);
	printf("\tNumber of Writes: %d\n", ptr -> no_of_writes);
	printf("\tSeek Time: ");
	print_double(ptr -> seek_time);
	printf("\n\tRotate Time: ");
	print_double(ptr -> rotate_time);
	printf("\n\tTransfer Time: ");
	print_double(ptr -> transfer_time);
	printf("\n\tTotal Time: ");
	print_double(ptr -> seek_time + ptr -> rotate_time + ptr -> transfer_time);
	printf("\n\n");
	restore(ps);
}

