#include <stdio.h>
#include <kernel.h>

/* 
 * These functions aim at printing floating point value (double).
 * The functions have potential performance problem.
 * Parameters:
 *     data: a floating point value to be printed
 */
void print_double(double data) {
	int upper, index;

	upper = (int)data;
	printf("%d.", upper);
	data -= upper;
	for(index = 0;index < 8;index++) {
		data *= 10.0;
		printf("%d", (int)data);
		data -= (int)data;
	}
}

void kprint_double(double data) {
	STATWORD ps;

	disable(ps);
	print_double(data);
	restore(ps);
}
