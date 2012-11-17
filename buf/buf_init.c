#include <kernel.h>
#include <pa4.h>
#include <buf.h>

/* 
 * Part A 1/4. buf_init()
 * buf_init() is responsible for initializing buffer cache management.
 */
void buf_init(void) {
	buf_head = (dsk_buffer_p)NULL;
	buf_count = 0;
	buf_is_open = 1;
}
