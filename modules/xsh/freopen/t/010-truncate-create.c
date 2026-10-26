/*
 * freopen() uses the mode as fopen() does: create, truncate, append.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.freopen.010
 * MUTANT    freopen.no_truncate
 * MUTANT    freopen.mode_0600
 * VARIANT   setvbuf.tiny_default
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
	pc_begin("XSH.freopen.010");
	{
		struct stat before, st;
		char buf[16];
		FILE *f, *g, *h;

		(void)umask(022);
		pc_make_file("old", "content");
		pc_check(chmod("old", 0600) == 0 && stat("old", &before) == 0, "cannot set up \"old\"");
		pc_make_file("keep", "keep");
		pc_make_file("s", "s");
		f = open_as("s", "r");
		g = open_as("s", "r");
		h = open_as("s", "r");
		pc_test();
		pc_check(freopen("new", "w", f) == f, "freopen(\"new\", \"w\") failed: %s", pc_errname(errno));
		pc_check(pc_file_mode("new") == 0644, "the new file has mode %#o, expected 0644", pc_file_mode("new"));
		pc_check(freopen("old", "w", g) == g, "freopen(\"old\", \"w\") failed: %s", pc_errname(errno));
		NEED(stat("old", &st) == 0, "stat failed");
		pc_check(st.st_size == 0, "\"w\" left the existing file %ld bytes long", (long)st.st_size);
		pc_check(st.st_ino == before.st_ino && st.st_dev == before.st_dev, "\"w\" replaced the existing file");
		pc_check((st.st_mode & 07777) == 0600, "\"w\" changed the file's mode to %#o", (unsigned)(st.st_mode & 07777));
		pc_check(freopen("keep", "a", h) == h, "freopen(\"keep\", \"a\") failed: %s", pc_errname(errno));
		(void)slurp("keep", buf, sizeof buf);
		pc_check(strcmp(buf, "keep") == 0, "\"a\" changed the file to \"%s\"", buf);
		NEED(fputs("Z", h) >= 0 && fclose(h) == 0, "writing failed");
		(void)slurp("keep", buf, sizeof buf);
		pc_check(strcmp(buf, "keepZ") == 0, "after appending \"Z\" the file holds \"%s\"", buf);
	}
	pc_pass();
}
