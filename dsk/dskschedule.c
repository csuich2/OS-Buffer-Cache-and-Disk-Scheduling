#include <kernel.h>
#include <pa4.h>

void insertSSTFRequest(disk_desc *ptr, request_desc_p req);
void insertCLOOKRequest(disk_desc *ptr, request_desc_p req);

/*
 * Part B 1/1. dskschedule()
 * This function should be implemented to support each disk scheduling policy.
 * Parameters:
 *     ptr:	a descriptor of each device information
 *     option:	a disk scheduling policy to be used
 */
void dskschedule(disk_desc *ptr, int option) {
	if (option == DISK_SCHED_FIFO || ptr->request_head == (request_desc_p)NULL)
		return;
	else if (option == DISK_SCHED_SSTF) {
		request_desc_p curr, next;
		curr = ptr->request_head;
		ptr->request_head = (request_desc_p)NULL;

		while (curr != (request_desc_p)NULL) {
			next = curr->next;
			curr->next = (request_desc_p)NULL;
			insertSSTFRequest(ptr, curr);
			curr = next;
		}
		return;
	} else if (option == DISK_SCHED_CLOOK) {
		request_desc_p curr, next;
		curr = ptr->request_head;
		ptr->request_head = (request_desc_p)NULL;

		while (curr != (request_desc_p)NULL) {
			next = curr->next;
			curr->next = (request_desc_p)NULL;
			insertCLOOKRequest(ptr, curr);
			curr = next;
		}
		return;
	}
}

void insertSSTFRequest(disk_desc *ptr, request_desc_p req) {
	request_desc_p curr, prev;
	int pos;

	pos = ptr->head_sector;
	curr = ptr->request_head;
	prev = (request_desc_p)NULL;

	if (curr == (request_desc_p)NULL) {
		ptr->request_head = req;
		return;
	}

	while (curr != (request_desc_p)NULL) {
		if (abs(curr->block_no - pos) < abs(req->block_no - pos)) {
			if (prev == (request_desc_p)NULL) {
				req->next = curr;
				ptr->request_head = req;
			} else {
				req->next = curr;
				prev->next = req;
			}
			return;
		}

		prev = curr;
		curr = curr->next;
	}

	prev->next = req;
}

void insertCLOOKRequest(disk_desc *ptr, request_desc_p req) {
	request_desc_p curr, prev;
	int pos;

	pos = ptr->head_sector;
	curr = ptr->request_head;
	prev = (request_desc_p)NULL;

	if (curr == (request_desc_p)NULL) {
		ptr->request_head = req;
		return;
	}

	while (curr != (request_desc_p)NULL) {
		int currBefore = (curr->block_no < pos);
		int reqBefore  = (req->block_no  < pos);
		if ((currBefore && reqBefore && curr->block_no < req->block_no) ||
		    (!currBefore && !reqBefore && curr->block_no < req->block_no) ||
		    (!currBefore && reqBefore)) {
			if (prev == (request_desc_p)NULL) {
				req->next = curr;
				ptr->request_head = req;
			} else {
				req->next = curr;
				prev->next = req;
			}
			return;
		}

		prev = curr;
		curr = curr->next;
	}

	prev->next = req;
}
