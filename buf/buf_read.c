#include <kernel.h>
#include <pa4.h>

extern dsk_buffer_p buf_get(void *pdev, int block_no);
extern dsk_buffer_p buf_fetch(struct devsw *pdev, int block_no, int policy);

/*
 * buf_read() checks the correctness of each parameter value,
 *            call buf_fetch() to read the request block,
 *            and copy the block data into the user buffer.
 * parameters:
 *            pdev:	device descriptor
 *            buffer:	a user buffer that should store the requested block
 *            block_no:	the block number to be served
 */
int buf_read(struct devsw *pdev, char *buffer, int block_no) {
	disk_desc *ptr;
	dsk_buffer_p target;
	int code;
	STATWORD ps;

	disable(ps);
	if(!pdev || !buffer) {
		restore(ps);
		return SYSERR;
	}

	ptr = (disk_desc *)pdev -> dvioblk;
	if(block_no < 0 || block_no >= ptr -> logical_blocks) {	
		restore(ps);
		return SYSERR;
	}

	restore(ps);
	if((target = buf_fetch(pdev, block_no, PA4_BUFFER_REPLACEMENT)) == INVALID_BLOCK) 
		return SYSERR;
	
	code = memncpy(buffer, target -> data, target -> size);
	return code;
}
