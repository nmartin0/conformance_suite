/*
 * A mapped file's contents persist after its last close.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.close.013
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static void sleep_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* Waits up to ms milliseconds for *flag to become non-zero. */
static int wait_for(volatile sig_atomic_t *flag, long ms)
{
	long i;

	for (i = 0; i < ms / 10 && !*flag; i++)
		sleep_ms(10);
	return *flag != 0;
}

static long ms_since(const struct timespec *t0)
{
	struct timespec t;

	(void)clock_gettime(CLOCK_MONOTONIC, &t);
	return (long)(t.tv_sec - t0->tv_sec) * 1000 + (t.tv_nsec - t0->tv_nsec) / 1000000;
}

int main(void)
{
	pc_begin("XSH.close.013");
	{
		long pg = sysconf(_SC_PAGESIZE);
		char *p, buf[8];
		int fd;

		pc_check(pg > 0, "sysconf failed");
		fd = open("f", O_RDWR | O_CREAT | O_EXCL, 0600);
		pc_check(fd >= 0 && write(fd, "abcdefgh", 8) == 8 && ftruncate(fd, pg) == 0, "could not make the file");
		p = mmap(NULL, (size_t)pg, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		pc_check(p != MAP_FAILED, "mmap failed: %s", pc_errname(errno));
		pc_test();
		pc_check(close(fd) == 0, "close failed: %s", pc_errname(errno));
		pc_check(memcmp(p, "abcdefgh", 8) == 0, "the mapping no longer holds the file's data");
		memcpy(p, "ZY", 2);
		pc_check(munmap(p, (size_t)pg) == 0, "munmap failed");
		fd = open("f", O_RDONLY);
		pc_check(fd >= 0 && read(fd, buf, 8) == 8, "reopening failed");
		pc_check(memcmp(buf, "ZYcdefgh", 8) == 0, "the change through the mapping is not in the file");
	}
	pc_pass();
}
