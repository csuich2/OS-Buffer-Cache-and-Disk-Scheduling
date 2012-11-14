#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <disk.h>

#define BUCKET_SIZE	1024

volatile int process_count = 0;
static int bucket[BUCKET_SIZE];

void	halt();

int	skewed_rand(void);
Bool	test1(void);
Bool	test2(void);
Bool	test3(void);
Bool	test4(void);
Bool	test5(void);
Bool	test6(void);

int	process1(char c);
int	process2(char c);
int	process3(char c);

int main() {
	Bool success = FALSE;

	test1();
//	test2();
//	test3();
//	test4();
//	test5();
//	test6();

	return 0;
}

Bool test1(void) {
	int processA, processB;
	int disk = DISK0;

	open(disk, 0, 0);

	resume(processA = create(process1, 2000, 30, "Process A", 1, (char)disk));
	resume(processB = create(process1, 2000, 30, "Process B", 1, (char)disk));

	while(process_count < 2);
	close(disk);

	return TRUE;
}

Bool test2(void) {
	int processA, processB;
	int disk = DISK0;

	open(disk, 0, 0);
	
	resume(processA = create(process2, 2000, 30, "Process A", 1, (char)disk));
	resume(processB = create(process2, 2000, 30, "Process B", 1, (char)disk));

	while(process_count < 2);
	close(disk);

	return TRUE;
}

Bool test3(void) {
	int process[8];
	int index;
	int disk = DISK0;

	open(disk, 0, 0);

	for(index = 0;index < 8;index++)
		resume(process[index] = create(process1, 2000, 30, "Process", 1, (char)disk));

	while(process_count < 8);
	close(disk);

	return TRUE;
}

Bool test4(void) {
	int process[8];
	int index;
	int disk = DISK0;

	open(disk, 0, 0);

	for(index = 0;index < 8;index++)
		resume(process[index] = create(process2, 2000, 30, "Process", 1, (char)disk));

	while(process_count < 8);
	close(disk);

	return TRUE;
}

Bool test5(void) {
	int process[8];
	int index;
	int disk = DISK0;

	open(disk, 0, 0);

	for(index = 0;index < 8;index++) 
		resume(process[index] = create(process3, 2000, 30, "Process", 1, (char)disk));

	while(process_count < 8);
	close(disk);

	return TRUE;
}

Bool test6(void) {
	char buffer[128];
	int disk = DISK0;

	open(disk, 0, 0);

	read(disk, buffer, 10);
	buffer[0] = 'H';
	buffer[1] = 'e';
	buffer[2] = 'l';
	buffer[3] = 'l';
	buffer[4] = 'o';
	buffer[5] = '!';
	buffer[6] = '\0';
	write(disk, buffer, 10);

	read(disk, buffer, 11);
	read(disk, buffer, 10);
	kprintf("expecting 'Hello!': '%s'\n", buffer);

	close(disk);

	return TRUE;
}

int skewed_rand(void) {
	int index, random;

	while(1) {
		random = rand() % BUCKET_SIZE;
		bucket[random]++;
		for(index = 0;index < BUCKET_SIZE;index++) {
			if(bucket[index] == index + 1) {
				bucket[index] = 0;
				return index;
			}
		}
	}
}

int process1(char c) {
	int index, random;
	char ibuffer[128];
	int disk = (int)c;

	kprintf("got here (process %d)\n", currpid);
	for(index = 0;index < 1024;index++) {
		random = rand() % 1024;
		read(disk, ibuffer, random);
		kprintf("finished read %d [%d]\n", index, currpid);
	}
	//kprintf("got here (the end)\n");

	process_count++;
	return process_count;
}

int process2(char c) {
	int index, random;
	char ibuffer[128];
	int disk = (int)c;

	for(index = 0;index < 1024;index++) {
		random = skewed_rand();
		read(disk, ibuffer, random);
	}
	
	process_count++;
	return process_count;
}

int process3(char c) {
	int index, random;
	char obuffer[128];
	int disk = (int)c;

	for(index = 0;index < 1024;index++) {
		random = rand() % 1024;
		obuffer[0] = random;
		write(disk, obuffer, random);
	}
	process_count++;
	return process_count;
}

