#include <pa4.h>

static int find_seek(disk_desc *pdesc, int sector);

/*
 * calculate_time() is responsible for calculating seek time and rotate time.
 * parameters:
 *     pdesc:		descriptor for each disk profile information
 *     block_no:	the disk block number
 *     seek_time:	the address of a variable to store calculated seek time
 *     rotate_time:	the address of a variable to store calculated roatet time
 */
void calculate_time(disk_desc *pdesc, int block_no, double *seek_time, double *rotate_time) {
	double spms = pdesc -> speed * (double)pdesc -> outer_track_size * (double)60;
	int target_track = block_no / pdesc -> outer_track_size;
	int current_track = pdesc -> head_sector / pdesc -> outer_track_size;
	int left_sectors, arrival;

	*seek_time = *rotate_time = 0.0;

	if(target_track != current_track) {
		int index = find_seek(pdesc, (pdesc -> head_sector > block_no) ? pdesc -> head_sector - block_no : block_no - pdesc -> head_sector);

		*seek_time = pdesc -> seeks[index].time;
	}
	//arrival = (pdesc -> head_sector + (int)(*seek_time * spms)) % pdesc -> outer_track_size;
	//left_sectors = (block_no - arrival) % pdesc -> outer_track_size;
	//while(left_sectors < 0)
	//	left_sectors += pdesc -> outer_track_size;

	left_sectors = pdesc -> outer_track_size / 2;
	*rotate_time = left_sectors / spms; 

	pdesc -> seek_time += *seek_time;
	pdesc -> rotate_time += *rotate_time;
}

/*
 * calculate_transfer_time() is responsible for calculating transfer time.
 * parameters:
 *     pdesc:		descriptor for each disk profile information
 *     block_no:	the disk block number
 *     count:		the number of blocks to be handled at the same time
 *     transfer_time:	the address of a variable to store calculated transfer time
 */
void calculate_transfer_time(disk_desc *pdesc, int block_no, int count, double *transfer_time) {
	int bytes = pdesc -> block_size * count;
	double rate = pdesc -> read.rate * (double)(1024 * 1024);

	*transfer_time = bytes / rate * 1000;

	pdesc -> transfer_time += *transfer_time;
}

/*
 * find_seek() is used to find a seek time based on the profile data.
 */
static int find_seek(disk_desc *pdesc, int sector) {
	int index = pdesc -> number_of_seeks >> 1;
	int step = pdesc -> number_of_seeks >> 2;

	if(pdesc -> number_of_seeks == 1)
		return 0;

	while(step != 0) {
		if(pdesc -> seeks[index].distance == sector)
			return index;
		else if(pdesc -> seeks[index].distance < sector) {
			index += step;
			step >>= 1;
		}
		else {
			index -= step;
			step >>= 1;
		}
	}
	while(index < pdesc -> number_of_seeks - 1 && pdesc -> seeks[index].distance < sector)index++;
	while(index > 0 && pdesc -> seeks[index].distance > sector)index--;

	return index;
}

/*
 * This function changes ms (seek time + rotate time + transfer time) to ticks proportional
 * to ms. The tick value is adjusted such that 10 <= ticks < 100. So, this does not mean
 * real-time.
 */
int ms_to_ticks(double ms) {
	ms *= 10.0;
	
	return ((int)ms + 10) % 100;
}
