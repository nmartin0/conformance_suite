/*
 * A pread() made while another thread pwrite()s the same bytes sees all
 * or none of that write.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.031
 *
 * One thread alternately writes a block of 16384 'A' and a block of 16384
 * 'B' at offset 0 of a file with pwrite() for 1.5 s; two threads read the
 * block with pread() as often as they can. Every block read must be
 * uniform and complete.
 */
#include "pcth.h"
#include <fcntl.h>

#define CH 16384

static int fd;

static void *writer(void *a)
{
	static unsigned char x[CH], y[CH];
	long end = now_ms() + 1500;
	int n = 0;

	(void)a;
	memset(x, 'A', CH);
	memset(y, 'B', CH);
	flag_set(0, 1);
	while (now_ms() < end) {
		int i;

		for (i = 0; i < 50; i++)
			if (pwrite(fd, (n++ & 1) ? x : y, CH, 0) != CH)
				flag_add(2, 1);
	}
	flag_set(1, 1);
	return NULL;
}

static void *reader(void *a)
{
	unsigned char b[CH];
	int j;

	(void)a;
	while (!flag_get(1)) {
		if (pread(fd, b, CH, 0) != CH) {
			flag_add(2, 1);
			continue;
		}
		for (j = 1; j < CH; j++)
			if (b[j] != b[0]) {
				flag_add(3, 1);
				break;
			}
		flag_add(4, 1);
	}
	return NULL;
}

int main(void)
{
	pthread_t w, r1, r2;
	unsigned char init[CH];

	pc_begin("XSH.2.9-threads.031");
	guard(120);
	memset(init, 'A', CH);
	fd = open("blk", O_RDWR | O_CREAT | O_TRUNC, 0600);
	NEED(fd != -1, "open failed");
	NEED(write(fd, init, CH) == CH, "write failed");
	pc_test();
	r1 = start(reader, NULL);
	r2 = start(reader, NULL);
	w = start(writer, NULL);
	NEED(pthread_join(w, NULL) == 0, "pthread_join failed");
	NEED(pthread_join(r1, NULL) == 0, "pthread_join failed");
	NEED(pthread_join(r2, NULL) == 0, "pthread_join failed");
	NEED(flag_get(2) == 0, "a pread() or pwrite() failed or was short");
	NEED(flag_get(4) >= 100, "the readers made fewer than 100 reads");
	pc_check(flag_get(3) == 0, "%d of %d pread() calls returned a block that mixed two pwrite() calls", flag_get(3), flag_get(4));
	pc_pass();
}
