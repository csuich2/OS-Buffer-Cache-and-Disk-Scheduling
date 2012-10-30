#ifndef _DISK_H


#define READ_OPERATION		0
#define WRITE_OPERATION		1

#define DISK_SCHED_FIFO 	0
#define DISK_SCHED_SSTF		1
#define DISK_SCHED_CLOOK	2

struct disk_op {
	int type;
	int block_no;
};

typedef struct disk_op 	disk_operation;

struct read_prf {
	int			start_block;
	double			rate;
};

typedef struct read_prf read_profile;

struct seek_prf {
	int 			distance;
	double 			time;		
};

typedef struct seek_prf seek_profile;

struct req_desc {
	int			type;
	int			block_no;
	int			process_id;
	int			ticks;
	char 			*buffer;
	int			count;
	struct req_desc		*next;
};

typedef struct req_desc		request_desc;
typedef request_desc		*request_desc_p;

struct dsk_desc {
	char			vendor[10];
	char			id[20];
	char			revision[5];
	int			logical_blocks;
	int			block_size;
	int			max_read;
	int			outer_track_size;
	double			speed;
	void			*disk;
	read_profile		read;
	int			number_of_seeks;
	seek_profile		*seeks;
	int			head_sector;
	double			seek_time;
	double 			rotate_time;
	double			transfer_time;
	int			no_of_reads;
	int			no_of_writes;
	request_desc_p		request_head;
};

typedef struct dsk_desc 	disk_desc;

#endif
