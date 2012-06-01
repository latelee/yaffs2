/*
 * YAFFS: Yet another FFS. A NAND-flash specific file system.
 *
 * Copyright (C) 2002-2011 Aleph One Ltd.
 *   for Toby Churchill Ltd and Brightstar Engineering
 *
 * Created by Charles Manning <charles@aleph1.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#include "yaffsfs.h"


const char *mp="yaffs2";
const char *fn = "yaffs2/fn";


void run_step(int test_type)
{
	int fd;
	int n;
	struct yaffs_stat s;
	unsigned char x;
	int i;

	printf("\n\n\nTest type %d\n", test_type);

	switch(test_type) {
	case 0:
	case 1:
		printf("Regular mount\n");
		yaffs_mount(mp);
		break;
	case 2:
		printf("mount ro checkpt\n");
		yaffs_mount_common(mp, 1, 0);
		break;
	case 3:
		printf("mount ro no-checkpt\n");
		yaffs_mount_common(mp, 1, 1);
		break;
	}

	if(test_type < 2) {
		if(test_type == 0)
			fd = yaffs_open(fn, O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE);
		else
			fd = yaffs_open(fn, O_CREAT | O_RDWR | O_APPEND, S_IREAD | S_IWRITE);

		if(fd <0) {
			printf("open failed\n");
			exit(1);
		}
		n = yaffs_write(fd, "foo bar ", 8);
		printf("write returned %d\n", n);
		yaffs_close(fd);
	}

	n = yaffs_stat(fn, &s);
	if(n < 0)
		printf("yaffs_stat returned %d\n", n);

	printf("file length %d\n", (int)s.st_size);

	fd = yaffs_open(fn, O_RDONLY, 0);

	for(i = 0; i < (int)s.st_size; i++) {
		yaffs_read(fd, &x, 1);
		printf("[%02x]", x);
	}
	printf("\n");

	yaffs_close(fd);

	n = yaffs_unmount(mp);

	printf("unmount returned %d\n", n);
}


int random_seed;
int simulate_power_failure;

int main(int argc, char *argv[])
{
	yaffs_start_up();
	run_step(0);
	run_step(1);
	run_step(2);
	run_step(3);

}
