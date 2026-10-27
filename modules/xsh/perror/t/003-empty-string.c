/*
 * perror() with an empty string writes the message and a newline only.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.perror.003
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
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* The file's whole contents (at most size - 1 bytes) and its length. */
static long slurp(const char *path, char *buf, size_t size)
{
	int fd = open(path, O_RDONLY);
	long n = 0, r;

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	while ((size_t)n < size - 1 && (r = read(fd, buf + n, size - 1 - (size_t)n)) > 0)
		n += r;
	(void)close(fd);
	buf[n] = '\0';
	return n;
}

/* Up to 16 bytes as hex, for messages. */
static const char *hex(const void *p, size_t n)
{
	static char out[16 * 3 + 1];
	const unsigned char *b = p;
	size_t i;

	out[0] = '\0';
	for (i = 0; i < n && i < 16; i++)
		snprintf(out + 3 * i, sizeof out - 3 * i, "%02x ", b[i]);
	return out;
}

static int saved_err = -1;

/*
 * Points descriptor 2 at path (created or truncated) for the calls that
 * follow; cap_end() puts it back. The test phase only: failures are
 * UNRESOLVED.
 */
static void cap_begin(const char *path)
{
	int fd;

	saved_err = dup(2);
	NEED(saved_err >= 0, "dup(2) failed: %s", pc_errname(errno));
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	NEED(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	NEED(dup2(fd, 2) == 2, "dup2 to 2 failed: %s", pc_errname(errno));
	NEED(close(fd) == 0, "close failed: %s", pc_errname(errno));
}

static void cap_end(void)
{
	(void)fflush(stderr);
	NEED(dup2(saved_err, 2) == 2, "restoring descriptor 2 failed: %s", pc_errname(errno));
	(void)close(saved_err);
}

/* Checks that file path holds exactly want. */
static void expect_text(const char *path, const char *want)
{
	char buf[512];
	long n = slurp(path, buf, sizeof buf);

	pc_check(n == (long)strlen(want) && memcmp(buf, want, (size_t)n) == 0,
	    "stderr received %ld bytes \"%s\", expected \"%s\"", n, buf, want);
}

int main(void)
{
	pc_begin("XSH.perror.003");
	{
		char want[256];

		pc_check(snprintf(want, sizeof want, "%s\n", strerror(EINVAL)) < (int)sizeof want, "message too long");
		pc_test();
		cap_begin("err");
		errno = EINVAL;
		perror("");
		cap_end();
		expect_text("err", want);
	}
	pc_pass();
}
