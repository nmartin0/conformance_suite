/*
 * Whatever marks a timestamp for update has taken effect by the time
 * fstat(), stat() or lstat() reports it: after a write, a truncation and a
 * mode change, the times show the current time (not the older value),
 * within the resolution.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.12-file-times-update.002
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

static void nap_ms(long ms)
{
	struct timespec ts, rem;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &rem) == -1 && errno == EINTR)
		ts = rem;
}

static void within(const char *what, struct timespec t, struct timespec lo, struct timespec hi)
{
	pc_check(ns(t) >= ns(lo) - 1000000000LL, "%s is %lld ns older than the operation, more than one second: the update was not done", what, ns(lo) - ns(t));
	pc_check(ns(t) <= ns(hi), "%s is %lld ns after the end of the checks", what, ns(t) - ns(hi));
}

int main(void)
{
	struct timespec t0, t1;
	struct stat s;
	int fd;

	pc_begin("XBD.4.12-file-times-update.002");
	(void)signal(SIGALRM, on_alarm);
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDWR);
	if (fd < 0)
		pc_unresolved("open failed: %s", pc_errname(errno));
	pc_test();
	(void)alarm(60);

	nap_ms(1300);
	(void)clock_gettime(CLOCK_REALTIME, &t0);
	pc_check(write(fd, "x", 1) == 1, "write failed");
	pc_check(fstat(fd, &s) == 0, "fstat failed: %s", pc_errname(errno));
	(void)clock_gettime(CLOCK_REALTIME, &t1);
	within("st_mtim after write() (fstat)", s.st_mtim, t0, t1);
	within("st_ctim after write() (fstat)", s.st_ctim, t0, t1);

	nap_ms(1300);
	(void)clock_gettime(CLOCK_REALTIME, &t0);
	pc_check(ftruncate(fd, 4) == 0, "ftruncate failed: %s", pc_errname(errno));
	pc_check(stat("f", &s) == 0, "stat failed: %s", pc_errname(errno));
	(void)clock_gettime(CLOCK_REALTIME, &t1);
	within("st_mtim after ftruncate() (stat)", s.st_mtim, t0, t1);
	within("st_ctim after ftruncate() (stat)", s.st_ctim, t0, t1);

	nap_ms(1300);
	(void)clock_gettime(CLOCK_REALTIME, &t0);
	pc_check(chmod("f", 0640) == 0, "chmod failed: %s", pc_errname(errno));
	pc_check(lstat("f", &s) == 0, "lstat failed: %s", pc_errname(errno));
	(void)clock_gettime(CLOCK_REALTIME, &t1);
	within("st_ctim after chmod() (lstat)", s.st_ctim, t0, t1);
	(void)close(fd);
	pc_pass();
}
