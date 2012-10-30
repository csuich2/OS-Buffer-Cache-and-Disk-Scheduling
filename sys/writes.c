#include <conf.h>
#include <kernel.h>
#include <io.h>

SYSCALL writes(int descrp, void *p, int block_no, int count) {
	struct devsw *devptr;

	if(isbaddev(descrp))
		return SYSERR;
	devptr = &devtab[descrp];
	return ((*devptr -> dvwrite)(devptr, p, block_no, count));
}
