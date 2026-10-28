/*
 * A store beyond the end of a mapped file, inside the last page, leaves the file size alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.8-realtime.001
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static void on_alarm(int s)
{
	(void)s;
	pc_fail("the test timed out");
}

int main(void)
{
	char data[100];
	long ps;
	int fd, i;
	volatile char *p;
	struct stat st;
	char back[100];

	pc_begin("XSH.2.8-realtime.001");
	for (i = 0; i < 100; i++)
		data[i] = (char)('a' + i % 26);
	ps = sysconf(_SC_PAGESIZE);
	NEED(ps > 160, "the page size is %ld", ps);
	fd = open("f", O_RDWR | O_CREAT | O_TRUNC, 0600);
	NEED(fd >= 0, "open failed: %s", pc_errname(errno));
	NEED(write(fd, data, 100) == 100, "write failed");
	p = mmap(NULL, (size_t)ps, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	NEED(p != MAP_FAILED, "mmap failed: %s", pc_errname(errno));
	(void)signal(SIGALRM, on_alarm);
	pc_test();
	(void)alarm(20);
	NEED(fstat(fd, &st) == 0, "fstat failed");
	pc_check(st.st_size == 100, "the file size is %ld before the store, not 100", (long)st.st_size);
	p[150] = 'x';
	(void)p[151];
	pc_check(fstat(fd, &st) == 0, "fstat failed");
	pc_check(st.st_size == 100, "after a store beyond the end the file size is %ld, not 100", (long)st.st_size);
	(void)msync((void *)p, (size_t)ps, MS_SYNC);
	pc_check(fstat(fd, &st) == 0 && st.st_size == 100, "after msync() the file size is not 100");
	pc_check(munmap((void *)p, (size_t)ps) == 0, "munmap failed");
	pc_check(fstat(fd, &st) == 0 && st.st_size == 100, "after munmap() the file size is not 100");
	pc_check(pread(fd, back, 100, 0) == 100 && memcmp(back, data, 100) == 0, "the first 100 bytes changed");
	pc_pass();
}
