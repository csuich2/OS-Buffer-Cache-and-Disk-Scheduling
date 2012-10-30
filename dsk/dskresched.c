#include <kernel.h>
#include <stdio.h>
#include <proc.h>
#include <q.h>

extern int ctxsw(int, int, int, int);

/*
 * This function makes the current process block state.
 */
int dskresched() {
	register struct pentry *optr;
	register struct pentry *nptr;
	STATWORD ps;

	disable(ps);

	optr = &proctab[currpid];
	optr -> pstate = PRSUSP;
	nptr = &proctab[(currpid = getlast(rdytail))];
	nptr -> pstate = PRCURR;

#ifdef RTCLOCK
	preempt = QUANTUM;
#endif
	restore(ps);
	ctxsw((int)&optr -> pesp, (int)optr -> pirmask, (int)&nptr -> pesp, (int)nptr -> pirmask);

	return OK;
}
