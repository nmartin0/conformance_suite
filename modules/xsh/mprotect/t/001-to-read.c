/*
 * After mprotect(PROT_READ), stores raise SIGSEGV and loads still work.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mprotect.001
 * MUTANT    mprotect.noop
 * VARIANT   mprotect.extra_read
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* The page size, from sysconf(_SC_PAGESIZE): never assumed. */
static long pg;

static void get_page_size(void)
{
	pg = sysconf(_SC_PAGESIZE);
	if (pg <= 0)
		pc_unresolved("sysconf(_SC_PAGESIZE) gave %ld", pg);
}

/* The byte at offset i of a test file. */
static unsigned char pat(long i)
{
	return (unsigned char)('A' + i % 23);
}

/* Write n bytes of buf at offset off of fd (setup: UNRESOLVED on error). */
static void put_bytes(int fd, const unsigned char *buf, long n, long off)
{
	while (n > 0) {
		ssize_t w = pwrite(fd, buf, (size_t)n, (off_t)off);
		if (w < 0 && errno == EINTR)
			continue;
		if (w <= 0)
			pc_unresolved("pwrite failed: %s", pc_errname(errno));
		buf += w;
		n -= (long)w;
		off += (long)w;
	}
}

/* Create path with n bytes, byte i being pat(i); returns an O_RDWR descriptor. */
static int make_data_file(const char *path, long n)
{
	unsigned char buf[512];
	long i, done;
	int fd = open(path, O_RDWR | O_CREAT | O_EXCL, 0600);

	if (fd < 0)
		pc_unresolved("cannot create %s: %s", path, pc_errname(errno));
	for (done = 0; done < n; done += (long)sizeof buf) {
		long k = n - done < (long)sizeof buf ? n - done : (long)sizeof buf;
		for (i = 0; i < k; i++)
			buf[i] = pat(done + i);
		put_bytes(fd, buf, k, done);
	}
	return fd;
}

/* Read n bytes at offset off of fd into buf; returns the count read. */
static long get_bytes(int fd, unsigned char *buf, long n, long off)
{
	long got = 0;

	while (got < n) {
		ssize_t r = pread(fd, buf + got, (size_t)(n - got), (off_t)(off + got));
		if (r < 0 && errno == EINTR)
			continue;
		if (r <= 0)
			break;
		got += (long)r;
	}
	return got;
}

/* The size of the file open on fd (UNRESOLVED if fstat() fails). */
static long fd_size(int fd)
{
	struct stat st;

	if (fstat(fd, &st) != 0)
		pc_unresolved("fstat failed: %s", pc_errname(errno));
	return (long)st.st_size;
}

/*
 * Touch one byte in a child process, with SIGSEGV and SIGBUS at their
 * default actions, unblocked, and no core file. With wr, store val; else
 * load the byte and, when val >= 0, compare it with val. Returns 0 when the
 * access completed (and the byte matched), the number of the signal that
 * ended the child, -2 when the byte loaded differed, and -1 otherwise.
 */
static int touch(volatile unsigned char *p, int wr, int val)
{
	pid_t kid;
	int st;

	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		struct rlimit rl;
		sigset_t set;

		rl.rlim_cur = rl.rlim_max = 0;
		(void)setrlimit(RLIMIT_CORE, &rl);
		(void)signal(SIGSEGV, SIG_DFL);
		(void)signal(SIGBUS, SIG_DFL);
		sigemptyset(&set);
		sigaddset(&set, SIGSEGV);
		sigaddset(&set, SIGBUS);
		(void)sigprocmask(SIG_UNBLOCK, &set, NULL);
		if (wr) {
			*p = (unsigned char)val;
			_exit(0);
		} else {
			unsigned char c = *p;
			_exit(val < 0 || c == (unsigned char)val ? 0 : 3);
		}
	}
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (WIFEXITED(st))
		return WEXITSTATUS(st) == 0 ? 0 : WEXITSTATUS(st) == 3 ? -2 : -1;
	if (WIFSIGNALED(st))
		return WTERMSIG(st);
	return -1;
}

/* What touch() returned, in words. */
static const char *how(int r)
{
	static char buf[64];

	if (r == 0)
		return "the access completed";
	if (r == -2)
		return "the byte loaded had another value";
	if (r == SIGSEGV)
		return "the child was killed by SIGSEGV";
	if (r == SIGBUS)
		return "the child was killed by SIGBUS";
	if (r > 0) {
		snprintf(buf, sizeof buf, "the child was killed by signal %d", r);
		return buf;
	}
	return "the child ended unexpectedly";
}

/* An anonymous MAP_SHARED mapping of n pages, read-write, each byte 'x' (setup). */
static unsigned char *anon_pages(long n)
{
	void *p = mmap(NULL, (size_t)(n * pg), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	if (p == MAP_FAILED)
		pc_unresolved("anonymous mmap of %ld pages failed: %s", n, pc_errname(errno));
	memset(p, 'x', (size_t)(n * pg));
	return p;
}

int main(void)
{
	pc_begin("XSH.mprotect.001");
	{
		unsigned char *a;
		int r;

		get_page_size();
		a = anon_pages(1);
		pc_test();
		r = mprotect(a, (size_t)pg, PROT_READ);
		pc_check(r == 0, "mprotect(PROT_READ) returned %d (%s)", r, pc_errname(errno));
		r = touch(a + 1, 1, 'w');
		pc_check(r == SIGSEGV, "a store after mprotect(PROT_READ): %s, not SIGSEGV", how(r));
		pc_check(a[1] == 'x', "the byte changed to %#x", a[1]);
	}
	pc_pass();
}
