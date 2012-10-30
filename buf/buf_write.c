#include <kernel.h>
#include <pa4.h>

extern dsk_buffer_p buf_get(void *pdev, int block_no);
extern dsk_buffer_p buf_victim(void);
extern dsk_buffer_p buf_alloc(int size);

extern int dskwrite(struct devsw *pdev, char *buffer, int block_no, int count);

extern int memncpy(void *dest, void *src, int num);

/*
 * buf_write() processes write requests.
 *     Firstly, this checks the correctness of parameters,
 *     and calls buf_put() to process actual requests according to policy
 *     (POLICY_WRITE_THROUGH or POLICY_DELAYED_WRITE).
 *
 * parameters:
 *     pdev:		device descriptor
 *     buffer:  	a user buffer that should hold the block_no block
 *     block_no:	block number to be served
 */
int buf_write(struct devsw *pdev, char *buffer, int block_no) {
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
	if(block_no < 0 || block_no > ptr -> logical_blocks) {
		restore(ps);
		return SYSERR;
	}
	restore(ps);
	code = buf_put(pdev, block_no, buffer, PA4_WRITE_POLICY);

	return code;
}
