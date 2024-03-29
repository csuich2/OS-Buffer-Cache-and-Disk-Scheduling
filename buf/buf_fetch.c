#include <kernel.h>
#include <stdio.h>
#include <pa4.h>
#include <buf.h>

extern int dskread(struct devsw *pdev, char *buffer, int block_no, int count);
extern int dskwrite(struct devsw *pdev, char *buffer, int block_no, int count);

extern int memncpy(void *dest, void *src, int num);

dsk_buffer_p findExistingBuffer(struct devsw *pdev, int block_no, int policy, int updateUsage);
dsk_buffer_p bringBlockIntoBuffer(struct devsw *pdev, int block_no, int performRead, STATWORD *ps);

/*
 * Part A 3/4. buf_fetch()
 * buf_fetch() does:
 *     if there is a buffer that already store the block data of block_no,
 *         then return the buffer.
 *     otherwise
 *         read the block of block_no, store it on a buffer, and return the buffer.
 * 
 * parameters:
 *     pdev:		device descriptor
 *     block_no: 	a block number to read
 *     policy: 		buffer replacement policy
 */
dsk_buffer_p buf_fetch(struct devsw *pdev, int block_no, int policy) {
	disk_desc *ptr;
	dsk_buffer_p target, prefetch;
	STATWORD ps;
	int i;

	disable(ps);

	// Check to make sure the devices is not null
	if (!pdev || !buf_is_open) {
		restore(ps);
		return (dsk_buffer_p)INVALID_BLOCK;
	}
	
	// Get the disk_desc and check that the requested block
	// number is valid
	ptr = (disk_desc *)pdev->dvioblk;
	if (block_no < 0 || block_no >= ptr->logical_blocks) {
		restore(ps);
		return (dsk_buffer_p)INVALID_BLOCK;
	}

	// Check to see if this block already exists in the buffer and update
	// its usage if policy is LRU
	target = findExistingBuffer(pdev, block_no, policy, 1);

	// If target != NULL, then cache hit occured, we can return the block
	if (target != (dsk_buffer_p)NULL) {
		// If this block is not valid, that means someone else is reading or writing
		// the block into the cache, so keep sleeping until it is valid
		restore(ps);
		while (!target->valid) {
			sleep(1);
		}
		// Once the block is valid, return
		return target;
	}

	// If we get here, then we did not find the block in the buffer and must bring
	// it into the buffer (and evict one if necessary)

	if ((target = bringBlockIntoBuffer(pdev, block_no, 1, ps)) == (dsk_buffer_p)INVALID_BLOCK) {
		return (dsk_buffer_p)INVALID_BLOCK;
	}

	// Now we need to prefetch blocks, which is a very similar process, but
	// slightly different (don't bail out if we find the block in memory already
	for (i=1; i<=PA4_PREFETCH_SIZE; i++) {
		// Bail out if we are about to prefetch a block_no larger
		// than the devices logical_blocks
		if (ptr->logical_blocks <= block_no+i)
			break;
	
		prefetch = findExistingBuffer(pdev, block_no+i, policy, 0);
		// If it does, bail out
		if (prefetch != (dsk_buffer_p)NULL) {
			continue;
		}

		// Otherwise, we need to bring it into the buffer
		if ((prefetch = bringBlockIntoBuffer(pdev, block_no+i, 1, ps)) == (dsk_buffer_p)INVALID_BLOCK) {
			restore(ps);
			// If something goes wrong, just return target, since that is they block
			// that was actually asked for
			return (dsk_buffer_p)target;
		}
	}

	restore(ps);
	return target;
}


dsk_buffer_p findExistingBuffer(struct devsw *pdev, int block_no, int policy, int updateUsage) {
	dsk_buffer_p target, previous;

	// While looping through, keep a reference to the current item and the
	// previous item in the linked list
	target = buf_head;
	previous = (dsk_buffer_p)NULL;
	// Loop over all current blocks
	while (target != (dsk_buffer_p)NULL) {
		// If this buffer is for the requested device and block number, then
		// we have found the block in the buffer
		if (target->pdev == pdev && target->block_no == block_no) {
			// If the policy is LRU, we need to move this block to the head
			// of the linked list, since it is now the most recently used
			if (policy == POLICY_LRU && previous != (dsk_buffer_p)NULL) {
				// If we should update this buffers usage
				if (updateUsage) {
					// Point the previous item to the next item
					previous->next = target->next;
					// Point the next item of the current block to the current head
					target->next = buf_head;
					// Point the head to the current block
					buf_head = target;
				}
			}
			// Restore interupts and return this block
			return target;
		}
		// Otherwise keep looking for the requested block
		previous = target;
		target = target->next;
	}

	return (dsk_buffer_p)NULL;
}

dsk_buffer_p bringBlockIntoBuffer(struct devsw *pdev, int block_no, int performRead, STATWORD *ps) {
	dsk_buffer_p target, previous;

	// If the buffer is full, we need to evict an entry
	if (buf_count == PA4_BUFFER_SIZE) {
		// Regardless of the policy, we will want to evict the last entry in the
		// linked list, because the FIFO policy wants the oldest entry to be
		// removed and LRU moves entrys to the front as their used, so the LRU entry
		// will be at the end of the list
		target = buf_head;
		previous = (dsk_buffer_p)NULL;
		while (target->next != (dsk_buffer_p)NULL) {
			previous = target;
			target = target->next;
		}
		
		// If the previous block isn't null, update its reference to the block
		// target points to, which is always NULL since target is the last block
		if (previous != (dsk_buffer_p)NULL)
			previous->next = (dsk_buffer_p)NULL;

		// We have now found the last entry and will replace the info in 'target'
		// with the info for the new entry
		
		// If the write policy is POLICY_DELAYED_WRITE and this entry is dirty, then
		// we need to write it to the disk
		if (PA4_WRITE_POLICY == POLICY_DELAYED_WRITE && target->dirty == 1) {
			target->valid = 0;
			restore(ps);
			dskwrite(target->pdev, target->data, target->block_no, 1);
			disable(ps);
			target->valid = 1;
		}
	// Otherwise, since the buffer is not full, we need to allocate a new buffer entry
	// and bump the buf_count variable
	} else {
		target = (dsk_buffer_p)getmem(sizeof(struct buf));
		// If there was an error allocating the new buffer entry, bail out
		if (target == (dsk_buffer_p)NULL) {
			return (dsk_buffer_p)INVALID_BLOCK;
		}
		// Allocate space for the data of this block
		target->data = (void*)getmem(128);
		if (target == (void*)NULL) {
			return (dsk_buffer_p)INVALID_BLOCK;
		}
		// Bump the buf_count
		buf_count++;
	}

	// Once we get here, the entry to put the new block in is referenced by 'target'
	
	// As long as the target and buf_head are not one in the same
	if (target != buf_head) {
		// So point this block to the current head
		target->next = buf_head;
		// And put the head to this block
		buf_head = target;
	}
	// Update the device
	target->pdev = pdev;
	// Update the block_no
	target->block_no = block_no;
	// Reset the dirty flag
	target->dirty = 0;
	// Always set the size to 128
	target->size = 128;
	// Set the valid flag to 0 since we're about to start reading it
	target->valid = 0;

	// If we want to read in the new block...
	if (performRead) {
		restore(ps);
		// Now read the data from the disk and set it into the target's data
		if (dskread(pdev, target->data, block_no, 1) == SYSERR) {
			disable(ps);
			return (dsk_buffer_p)INVALID_BLOCK;
		}
		disable(ps);
		// Now that the block is read and in the cache, it is valid
		target->valid = 1;
	}

	return target;
}
