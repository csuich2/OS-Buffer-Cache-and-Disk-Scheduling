#include <kernel.h>
#include <stdio.h>
#include <pa4.h>
#include <buf.h>

extern int dskread(struct devsw *pdev, char *buffer, int block_no, int count);
extern int dskwrite(struct devsw *pdev, char *buffer, int block_no, int count);

extern int memncpy(void *dest, void *src, int num);

extern dsk_buffer_p findExistingBuffer(struct devsw *pdev, int block_no, int policy, int updateUsage);
extern int bringBlockIntoBuffer(struct devsw *pdev, int block_no, dsk_buffer_p target);

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
	STATWORD ps;
	
	disable(ps);
	// Check to make sure the devices is not null
	if (!pdev) {
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

	// Check to see if this block already exists in the buffer and update
	// its usage if policy is LRU
	target = findExistingBuffer(pdev, block_no, policy, 1);
	if (target == (dsk_buffer_p)NULL) {
		int ret = bringBlockIntoBuffer(pdev, block_no, target);
		if (ret != OK) {
			restore(ps);
			return ret;
		}
	}

	// Copy the buffer data into this blocks data
	memncpy(target->data, buffer, 128);

	// Now that we have the buffer target, write the data to that block
	if (policy == POLICY_WRITE_THROUGH) {
		dskwrite(target->pdev, target->data, target->block_no, 1);
	} else { //if (policy == POLICY_DELAYED_WRITE) {
		target->dirty = 1;
	}

	restore(ps);
	return OK;
}
