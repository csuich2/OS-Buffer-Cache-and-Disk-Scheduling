#include <kernel.h>
#include <pa4.h>

extern int strcpy(char *dest, char *src);

/*
 * dskinit() set profile data into each device descriptor,
 *           and creates virtual disks whose size is decided by profile data.
 * parameters:
 *     pdev:	device descriptor
 */
int dskinit(struct devsw *pdev) {
	disk_desc *ptr;

	if(pdev -> dvioblk) 
		return SYSERR;

	pdev -> dvioblk = (char *)getmem(sizeof(disk_desc));
	ptr = (disk_desc *)pdev -> dvioblk;

	ptr -> head_sector	= 0;
	ptr -> seek_time	= 0.0;
	ptr -> rotate_time	= 0.0;
	ptr -> transfer_time	= 0.0;
	ptr -> no_of_reads	= 0;
	ptr -> no_of_writes	= 0;
	ptr -> request_head	= NULL;

	if(&devtab[DISK0] == pdev) {
		strcpy(ptr -> vendor, "IBM-ESXS");
		strcpy(ptr -> id, "DTN036C1UCDY10F");
		strcpy(ptr -> revision, "S23J");
		ptr -> logical_blocks		= 1024;
		ptr -> block_size		= 128;
		ptr -> max_read			= 128;
		ptr -> outer_track_size		= 32;
		ptr -> speed			= 10.2144;
		ptr -> disk			= (void *)getmem(ptr -> logical_blocks * ptr -> block_size);
		ptr -> read.start_block		= 0;
		ptr -> read.rate		= 0.665;
		ptr -> number_of_seeks		= 32;
		ptr -> seeks			= (seek_profile *)getmem(32 * sizeof(seek_profile));
		ptr -> seeks[0].distance	= 32;
		ptr -> seeks[0].time		= 2.496875;
		ptr -> seeks[1].distance	= 64;
		ptr -> seeks[1].time		= 2.70625;
		ptr -> seeks[2].distance	= 96;
		ptr -> seeks[2].time		= 3.0;
		ptr -> seeks[3].distance	= 128;
		ptr -> seeks[3].time		= 3.025;
		ptr -> seeks[4].distance	= 160;
		ptr -> seeks[4].time		= 3.815625;
		ptr -> seeks[5].distance	= 192;
		ptr -> seeks[5].time		= 4.325;
		ptr -> seeks[6].distance	= 224;
		ptr -> seeks[6].time		= 4.321875;
		ptr -> seeks[7].distance	= 256;
		ptr -> seeks[7].time		= 4.6;
		ptr -> seeks[8].distance	= 288;
		ptr -> seeks[8].time		= 4.828125;
		ptr -> seeks[9].distance	= 320;
		ptr -> seeks[9].time		= 5.1;
		ptr -> seeks[10].distance	= 352;
		ptr -> seeks[10].time		= 5.4;
		ptr -> seeks[11].distance	= 384;
		ptr -> seeks[11].time		= 5.6;
		ptr -> seeks[12].distance	= 416;
		ptr -> seeks[12].time		= 5.659375;
		ptr -> seeks[13].distance	= 448;
		ptr -> seeks[13].time		= 5.85625;
		ptr -> seeks[14].distance	= 480;
		ptr -> seeks[14].time		= 6.140625;
		ptr -> seeks[15].distance	= 512;
		ptr -> seeks[15].time		= 6.2;
		ptr -> seeks[16].distance	= 544;
		ptr -> seeks[16].time		= 6.50625;
		ptr -> seeks[17].distance	= 576;
		ptr -> seeks[17].time		= 6.6875;
		ptr -> seeks[18].distance	= 608;
		ptr -> seeks[18].time		= 6.959375;
		ptr -> seeks[19].distance	= 640;
		ptr -> seeks[19].time		= 7.125;
		ptr -> seeks[20].distance	= 672;
		ptr -> seeks[20].time		= 7.234375;
		ptr -> seeks[21].distance	= 704;
		ptr -> seeks[21].time		= 7.5;
		ptr -> seeks[22].distance	= 736;
		ptr -> seeks[22].time		= 7.671875;
		ptr -> seeks[23].distance	= 768;
		ptr -> seeks[23].time		= 7.9;
		ptr -> seeks[24].distance	= 800;
		ptr -> seeks[24].time		= 8.0;
		ptr -> seeks[25].distance	= 832;
		ptr -> seeks[25].time		= 8.28125;
		ptr -> seeks[26].distance	= 864;
		ptr -> seeks[26].time		= 8.5;
		ptr -> seeks[27].distance	= 896;
		ptr -> seeks[27].time		= 8.7875;
		ptr -> seeks[28].distance	= 928;
		ptr -> seeks[28].time		= 8.990625;
		ptr -> seeks[29].distance	= 960;
		ptr -> seeks[29].time		= 9.29375;
		ptr -> seeks[30].distance	= 992;
		ptr -> seeks[30].time		= 9.4;
		ptr -> seeks[31].distance	= 1024;
		ptr -> seeks[31].time		= 9.5;
	}
	else if(&devtab[DISK1] == pdev) {
		strcpy(ptr -> vendor, "SEAGATE");
		strcpy(ptr -> id, "ST336607LW");
		strcpy(ptr -> revision, "0007");
		ptr -> logical_blocks		= 1024;
		ptr -> block_size		= 128;
		ptr -> max_read			= 128;
		ptr -> outer_track_size		= 32;
		ptr -> speed			= 5.00736;
		ptr -> disk			= (void *)getmem(ptr -> logical_blocks * ptr -> block_size);
		ptr -> read.start_block		= 0;
		ptr -> read.rate		= 0.326;
		ptr -> number_of_seeks		= 32;
		ptr -> seeks			= (seek_profile *)getmem(32 * sizeof(seek_profile));
		ptr -> seeks[0].distance      	= 32;
                ptr -> seeks[0].time            = 2.596875;
                ptr -> seeks[1].distance        = 64;
                ptr -> seeks[1].time            = 3.00625;
                ptr -> seeks[2].distance        = 96;
                ptr -> seeks[2].time            = 3.390625;
                ptr -> seeks[3].distance        = 128;
                ptr -> seeks[3].time            = 3.5375;
                ptr -> seeks[4].distance        = 160;
                ptr -> seeks[4].time            = 4.215625;
                ptr -> seeks[5].distance        = 192;
                ptr -> seeks[5].time            = 4.21875;
                ptr -> seeks[6].distance        = 224;
                ptr -> seeks[6].time            = 4.65625;
                ptr -> seeks[7].distance        = 256;
                ptr -> seeks[7].time            = 4.65;
                ptr -> seeks[8].distance        = 288;
                ptr -> seeks[8].time            = 4.95625;
                ptr -> seeks[9].distance        = 320;
                ptr -> seeks[9].time            = 5.19375;
                ptr -> seeks[10].distance       = 352;
                ptr -> seeks[10].time           = 5.203125;
                ptr -> seeks[11].distance       = 384;
                ptr -> seeks[11].time           = 5.6625;
                ptr -> seeks[12].distance       = 416;
                ptr -> seeks[12].time           = 6.040625;
                ptr -> seeks[13].distance       = 448;
                ptr -> seeks[13].time           = 6.2375;
                ptr -> seeks[14].distance       = 480;
                ptr -> seeks[14].time           = 6.00625;
                ptr -> seeks[15].distance       = 512;
                ptr -> seeks[15].time           = 6.35;
                ptr -> seeks[16].distance       = 544;
                ptr -> seeks[16].time           = 6.846875;
                ptr -> seeks[17].distance       = 576;
                ptr -> seeks[17].time           = 7.2875;
                ptr -> seeks[18].distance       = 608;
                ptr -> seeks[18].time           = 7.4375;
                ptr -> seeks[19].distance       = 640;
                ptr -> seeks[19].time           = 7.1125;
                ptr -> seeks[20].distance       = 672;
                ptr -> seeks[20].time           = 7.690625;
                ptr -> seeks[21].distance       = 704;
                ptr -> seeks[21].time           = 7.5;
                ptr -> seeks[22].distance       = 736;
                ptr -> seeks[22].time           = 8.028125;
                ptr -> seeks[23].distance       = 768;
                ptr -> seeks[23].time           = 7.925;
                ptr -> seeks[24].distance       = 800;
                ptr -> seeks[24].time           = 8.0125;
                ptr -> seeks[25].distance       = 832;
                ptr -> seeks[25].time           = 8.11875;
                ptr -> seeks[26].distance       = 864;
                ptr -> seeks[26].time           = 8.384375;
                ptr -> seeks[27].distance       = 896;
                ptr -> seeks[27].time           = 8.7875;
		ptr -> seeks[28].distance	= 928;
		ptr -> seeks[28].time		= 8.746875;
		ptr -> seeks[29].distance	= 960;
		ptr -> seeks[29].time		= 9.5625;
		ptr -> seeks[30].distance	= 992;
		ptr -> seeks[30].time		= 9.4;
		ptr -> seeks[31].distance	= 1024;
		ptr -> seeks[31].time		= 9.5;
	}

	return OK;
}
