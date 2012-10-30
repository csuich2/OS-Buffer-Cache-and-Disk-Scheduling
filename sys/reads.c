#include <conf.h>
#include <kernel.h>
#include <io.h>

DEVCALL reads(int descrp, void *p, int block_no, int count) {
	struct devsw *devptr;

	if(isbaddev(descrp))
		return SYSERR;

	devptr = &devtab[descrp];
	return ((*devptr -> dvread)(devptr, p, block_no, count));
}

