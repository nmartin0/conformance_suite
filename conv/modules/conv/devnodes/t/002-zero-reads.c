/*
 * The zero device reads as zeros and discards what is written.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.devnodes.002
 */
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pc.h"
#include "../../../../lib/cv.h"

int main(void)
{
	char p[1024];
	static unsigned char buf[65536];
	int fd, i;
	size_t k;
	ssize_t r;

	pc_begin("CONV.devnodes.002");
	(void)cv_dev(p, sizeof p, "zero");
	fd = open(p, O_RDWR);
	pc_check(fd >= 0, "cannot open %s for reading and writing: errno %d", p, errno);
	pc_test();
	for (i = 0; i < 16; i++) {
		memset(buf, 0xA5, sizeof buf);
		r = read(fd, buf, sizeof buf);
		pc_check(r == (ssize_t)sizeof buf, "read of %zu bytes returned %zd", sizeof buf, r);
		for (k = 0; k < sizeof buf; k++)
			pc_check(buf[k] == 0, "byte %zu of a read is 0x%02x, not zero", k, buf[k]);
	}
	memset(buf, 0x5A, 4096);
	r = write(fd, buf, 4096);
	pc_check(r == 4096, "write of 4096 bytes returned %zd", r);
	r = read(fd, buf, 16);
	pc_check(r == 16 && buf[0] == 0 && buf[15] == 0, "a read after a write does not return zeros");
	pc_pass();
}
