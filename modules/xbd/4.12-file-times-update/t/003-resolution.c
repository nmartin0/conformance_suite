/*
 * The resolution of timestamps is no coarser than a second: changes made
 * more than a second apart give different modification and status-change
 * times.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.12-file-times-update.003
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

static void on_alarm(int s)
{
	(void)s;
	pc_fail("the test timed out");
}

static long long ns(struct timespec t)
{
	return (long long)t.tv_sec * 1000000000LL + t.tv_nsec;
}

static long long mono_ms(void)
{
	struct timespec t;

	(void)clock_gettime(CLOCK_MONOTONIC, &t);
	return (long long)t.tv_sec * 1000 + t.tv_nsec / 1000000;
}

int main(void)
{
	struct stat a, b;
	struct timespec nap = { 0, 100000000L };
	long long m0;
	int fd;

	pc_begin("XBD.4.12-file-times-update.003");
	(void)signal(SIGALRM, on_alarm);
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDWR);
	if (fd < 0)
		pc_unresolved("open failed: %s", pc_errname(errno));
	pc_test();
	(void)alarm(60);
	pc_check(write(fd, "a", 1) == 1 && fstat(fd, &a) == 0, "write or fstat failed");
	m0 = mono_ms();
	while (mono_ms() - m0 < 1200)
		(void)nanosleep(&nap, NULL);
	pc_check(write(fd, "b", 1) == 1 && fstat(fd, &b) == 0, "write or fstat failed");
	(void)close(fd);
	pc_check(ns(b.st_mtim) > ns(a.st_mtim), "st_mtim did not change between writes 1.2 s apart (%lld ns, then %lld ns)", ns(a.st_mtim), ns(b.st_mtim));
	pc_check(ns(b.st_ctim) > ns(a.st_ctim), "st_ctim did not change between writes 1.2 s apart (%lld ns, then %lld ns)", ns(a.st_ctim), ns(b.st_ctim));
	pc_pass();
}
