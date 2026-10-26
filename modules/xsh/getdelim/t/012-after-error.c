/*
 * getdelim() on a stream whose error indicator is set still reads a record.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdelim.012
 * MUTANT    getdelim.error_sticky
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
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* Sets O_NONBLOCK on fd (setup). */
static void nonblock(int fd)
{
	int fl = fcntl(fd, F_GETFL);

	pc_check(fl != -1 && fcntl(fd, F_SETFL, fl | O_NONBLOCK) == 0, "cannot set O_NONBLOCK: %s", pc_errname(errno));
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

int main(void)
{
	pc_begin("XSH.getdelim.012");
	{
		FILE *f;
		char *p = NULL;
		size_t n = 0;
		ssize_t r;
		int fd[2];

		if (!(pipe(fd) == 0))
			pc_unresolved("pipe failed: %s", pc_errname(errno));
		nonblock(fd[0]);
		pc_test();
		f = fdopen(fd[0], "r");
		NEED(f != NULL, "fdopen() failed: %s", pc_errname(errno));
		errno = 0;
		r = getdelim(&p, &n, ':', f);
		NEED(r == -1 && errno == EAGAIN && ferror(f), "getdelim() on the empty pipe did not fail with EAGAIN and set the error indicator");
		NEED(write(fd[1], "ab:", 3) == 3, "write to the pipe failed: %s", pc_errname(errno));
		errno = 0;
		r = getdelim(&p, &n, ':', f);
		pc_check(r == 3, "with the error indicator set and \"ab:\" in the pipe, getdelim() returned %ld (errno %s), expected 3", (long)r, pc_errname(errno));
		pc_check(p != NULL && memcmp(p, "ab:", 4) == 0, "the object holds %s, expected 61 62 3a 00", p ? hex(p, 4) : "nothing");
		free(p);
	}
	pc_pass();
}
