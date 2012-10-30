#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <pa4.h>

extern int disk0_preempt, disk1_preempt;

/*
 * These functions are called whenever corresponding virtual interrrupts occur (diskX_preempt == 0).
 * The main purpose is:
 *     to set MAXINT to diskX_preempt when each queue is empty
 *     to get the last request in each queue, serve it, and put the process that has made the request into ready queue
 *     to schedule next interrupt.
 */
int dsk0interrupt(void) {
	request_desc_p target, prev;
	disk_desc *ptr;
	int ticks;
	double seek_time, rotate_time, transfer_time;
	STATWORD ps;

	disable(ps);
	ptr = (disk_desc *)devtab[DISK0].dvioblk;
	if(!ptr) {
		restore(ps);
		return SYSERR;
	}

	if(!ptr -> request_head) {
		disk0_preempt = MAXINT;
		restore(ps);
		return OK;
	}

	for(target = ptr -> request_head, prev = NULL;target -> next;prev = target, target = target -> next); 
	if(prev != NULL)
		prev -> next = NULL;
	else
		ptr -> request_head = NULL;

	if(target -> type == READ_OPERATION) {
		ptr -> no_of_reads++;
		memncpy(target -> buffer, ptr -> disk + target -> block_no * ptr -> block_size, ptr -> block_size * target -> count);
	}
	else {
		ptr -> no_of_writes++;
		memncpy(ptr -> disk + target -> block_no * ptr -> block_size, target -> buffer, ptr -> block_size * target -> count);
	}
	ptr -> head_sector = target -> block_no + target -> count;
	ready(target -> process_id, RESCHNO);
	freemem(target, sizeof(request_desc));

	// Disk Schedule
	dskschedule(ptr, PA4_DISK_SCHEDULE);
	
	if(ptr -> request_head) {
		for(target = ptr -> request_head;target -> next;target = target -> next);
		calculate_time(ptr, target -> block_no, &seek_time, &rotate_time);
		calculate_transfer_time(ptr, target -> block_no, target -> count, &transfer_time);
		ticks = ms_to_ticks(seek_time + rotate_time + transfer_time);
		target -> ticks = ticks;
		disk0_preempt = ticks;
	}
	else
		disk0_preempt = MAXINT;

	restore(ps);
	return OK;
}

int dsk1interrupt(void) {
	request_desc_p target, prev;
	disk_desc *ptr;
	int ticks;
	double seek_time, rotate_time, transfer_time;
	STATWORD ps;

	disable(ps);
	ptr = (disk_desc *)devtab[DISK1].dvioblk;
	if(!ptr) {
		restore(ps);
		return SYSERR;
	}

	if(!ptr -> request_head) {
		disk1_preempt = MAXINT;
		restore(ps);
		return OK;
	}

	for(target = ptr -> request_head, prev = NULL;target -> next;prev = target, target = target -> next); 
	if(prev != NULL)
		prev -> next = NULL;
	else
		ptr -> request_head = NULL;

	if(target -> type == READ_OPERATION) {
		ptr -> no_of_reads++;
		memncpy(target -> buffer, ptr -> disk + target -> block_no * ptr -> block_size, ptr -> block_size * target -> count);
	}
	else {
		ptr -> no_of_writes++;
		memncpy(ptr -> disk + target -> block_no * ptr -> block_size, target -> buffer, ptr -> block_size * target -> count);
	}
	ptr -> head_sector = target -> block_no + target -> count;
	ready(target -> process_id, RESCHNO);
	freemem(target, sizeof(request_desc));

	// Disk Schedule
	dskschedule(ptr, PA4_DISK_SCHEDULE);
	
	if(ptr -> request_head) {
		for(target = ptr -> request_head;target -> next;target = target -> next);
		calculate_time(ptr, target -> block_no, &seek_time, &rotate_time);
		calculate_transfer_time(ptr, target -> block_no, target -> count, &transfer_time);
		ticks = ms_to_ticks(seek_time + rotate_time + transfer_time);
		target -> ticks = ticks;
		disk1_preempt = ticks;
	}
	else
		disk1_preempt = MAXINT;

	restore(ps);
	return OK;
}
