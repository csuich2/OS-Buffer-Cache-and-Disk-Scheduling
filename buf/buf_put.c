#include <kernel.h>
#include <stdio.h>
#include <pa4.h>
#include <buf.h>

extern int dskread(struct devsw *pdev, char *buffer, int block_no, int count);
extern int dskwrite(struct devsw *pdev, char *buffer, int block_no, int count);

extern int memncpy(void *dest, void *src, int num);

extern dsk_buffer_p findExistingBuffer(struct devsw *pdev, int block_no, int policy, int updateUsage);
extern dsk_buffer_p bringBlockIntoBuffer(struct devsw *pdev, int block_no, int performRead, STATWORD *ps);

/*
 * Part A 4/4. buf_put()
 * buf_put() processes each write request.
 * If policy is POLICY_WRITE_THROUGH,
 *     then find a buffer that already stores the block_no block, stores new data to it
 *     and finally store the data into disk.
 * Otherwise (POLICY_DELAYED_WRITE),
 *     then you have to handle requests based on the policy.
 */
int buf_put(struct devsw *pdev, int block_no, char *buffer, int policy) {
	disk_desc *ptr;
	dsk_buffer_p target, previous;
	STATWORD *ps;

	disable(ps);

	// Check to make sure the devices is not null
	if (!pdev || !buf_is_open) {
		restore(ps);
		return SYSERR;
	}

	// Check to make sure the policy is valid
	if (policy != POLICY_WRITE_THROUGH && policy != POLICY_DELAYED_WRITE) {
		restore(ps);
		return SYSERR;
	}

	// Get the disk_desc and check that the requested block
	// number is valid
	ptr = (disk_desc *)pdev->dvioblk;
	if (block_no < 0 || block_no >= ptr->logical_blocks) {
		restore(ps);
		return SYSERR;
	}

	// We need to check if the block is already in the cache
	target = findExistingBuffer(pdev, block_no, policy, 1);
	// If it is not
	if (target == (dsk_buffer_p)NULL) {
		// Bring it into the cache, but DON't submit a read request
		if ((target = bringBlockIntoBuffer(pdev, block_no, 0, ps)) == (dsk_buffer_p)INVALID_BLOCK) {
			return SYSERR;
		}
	}
	// Copy the passed buffer into the data
	memncpy(target->data, buffer, 128);

	// Write the data to that block, or just mark it as dirty
	if (policy == POLICY_WRITE_THROUGH) {
		target->valid = 0;
		restore(ps);
		dskwrite(pdev, buffer, block_no, 1);
		target->valid = 1;
	} else { //if (policy == POLICY_DELAYED_WRITE) {
		// Set the target as valid now that we have populated the data
		target->valid = 1;
		// Set the block as dirty
		target->dirty = 1;
		restore(ps); 
	}

	return OK;
}
