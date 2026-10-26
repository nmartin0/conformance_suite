/*
 * fsetpos() writes buffered output to the file and marks its times.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fsetpos.005
 * MUTANT    fsetpos.drops_pending
 * MUTANT    fsetpos.old_times
 * VARIANT   setvbuf.own_unused
 * VARIANT   setvbuf.scribbles
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static FILE *open_as(const char *path, const char *mode)
{
	FILE *f = fopen(path, mode);

	NEED(f != NULL, "fopen(%s, %s) failed: %s", path, mode, pc_errname(errno));
	return f;
}

/* The file's whole contents (at most size - 1 bytes) and its length. */
static long slurp(const char *path, char *buf, size_t size)
{
	int fd = open(path, O_RDONLY);
	long n = 0, r;

	NEED(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	while ((size_t)n < size - 1 && (r = read(fd, buf + n, size - 1 - (size_t)n)) > 0)
		n += r;
	(void)close(fd);
	buf[n] = '\0';
	return n;
}

static int ts_after(struct timespec a, struct timespec b)
{
	return a.tv_sec > b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_nsec > b.tv_nsec);
}

/*
 * Set the error indicator of a stream that has nothing buffered for input:
 * its descriptor is closed for one fgetc(), whose read fails with EBADF
 * (XSH fgetc(): data needs to be read), then put back with dup2(). The
 * copy is never used to affect the offset, so it is not a second handle
 * (XSH 2.5.1).
 */
static void set_error(FILE *f)
{
	int fd = fileno(f), d = dup(fd);

	NEED(fd >= 0 && d >= 0, "dup failed: %s", pc_errname(errno));
	NEED(close(fd) == 0, "close failed");
	NEED(fgetc(f) == EOF, "fgetc() on a closed descriptor returned a byte");
	NEED(dup2(d, fd) == fd && close(d) == 0, "dup2 failed: %s", pc_errname(errno));
	NEED(ferror(f), "fgetc() on a closed descriptor did not set the error indicator");
}

int main(void)
{
	pc_begin("XSH.fsetpos.005");
	{
		static char vb[BUFSIZ];
		struct stat before, st;
		char buf[16];
		fpos_t p0;
		FILE *f;

		pc_make_file("f", "old");
		f = open_as("f", "w");
		pc_check(setvbuf(f, vb, _IOFBF, sizeof vb) == 0, "setvbuf(_IOFBF) was refused");
		pc_check(fgetpos(f, &p0) == 0 && fstat(fileno(f), &before) == 0, "fgetpos or fstat failed");
		sleep(2);
		pc_test();	/* its fflush(NULL) would write the data meant to stay buffered */
		NEED(fputs("abc", f) >= 0, "fputs failed");
		NEED(pc_file_size("f") == 0, "the data was not buffered");
		pc_check(fsetpos(f, &p0) == 0, "fsetpos failed: %s", pc_errname(errno));
		(void)slurp("f", buf, sizeof buf);
		pc_check(strcmp(buf, "abc") == 0, "the file holds \"%s\" after fsetpos(), expected \"abc\"", buf);
		NEED(stat("f", &st) == 0, "stat failed");
		pc_check(ts_after(st.st_mtim, before.st_mtim), "the modification time was not updated");
		pc_check(ts_after(st.st_ctim, before.st_ctim), "the status change time was not updated");
	}
	pc_pass();
}
