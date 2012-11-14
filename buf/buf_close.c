#include <kernel.h>
#include <pa4.h>


/*
 * Part A 2/4. buf_close()
 * buf_close() deallocates all the resources that buffer manager has used.
 */
void buf_close(void) {
	dsk_buffer_p dbp = buf_head;
	while (dbp != (dsk_buffer_p)NULL) {
		dsk_buffer_p next = buf_head->next;
		freemem(dbp, sizeof(struct buf));
		dbp = next;
	}
}
