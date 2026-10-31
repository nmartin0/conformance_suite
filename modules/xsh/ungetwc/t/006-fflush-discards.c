/*
 * fflush() discards pushed-back characters.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ungetwc.006
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <locale.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* A stream on path in mode (a precondition). */
static FILE *open_as(const char *path, const char *mode)
{
	FILE *f = fopen(path, mode);

	NEED(f != NULL, "fopen(%s, %s) failed: %s", path, mode, pc_errname(errno));
	return f;
}

/*
 * Grows the file by 10 bytes through a new open file description, which is
 * not a handle on the stream's (XSH 2.5.1): however much the stream has
 * buffered, its descriptor is then not at end of file on either reading of
 * "not already at EOF" (Q-29), so fflush() must discard the pushback by its
 * own page as well as by this one (Q-47).
 */
static void grow(const char *path)
{
	int g = open(path, O_WRONLY | O_APPEND);

	NEED(g >= 0 && write(g, "0123456789", 10) == 10 && close(g) == 0, "growing the file failed: %s", pc_errname(errno));
}

int main(void)
{
	pc_begin("XSH.ungetwc.006");
	{
		FILE *f;
		wint_t c;

		{
			/* 1 MiB beginning "abc", grown after the read (see grow()) */
			static char big[1 << 20];
			int fd = open("f", O_WRONLY | O_CREAT | O_EXCL, 0644);

			memset(big, 'x', sizeof big);
			memcpy(big, "abc", 3);
			pc_check(fd >= 0 && write(fd, big, sizeof big) == (ssize_t)sizeof big && close(fd) == 0, "making f failed");
		}
		pc_test();
		f = open_as("f", "r");
		NEED(fgetwc(f) == L'a', "fgetwc() did not read L'a'");
		NEED(ungetwc(L'Z', f) == L'Z', "ungetwc() failed");
		grow("f");
		NEED(fflush(f) == 0, "fflush() failed: %s", pc_errname(errno));
		c = fgetwc(f);
		/* Which character comes next is not settled (Q-30); only the discarded one is wrong. */
		pc_check(c != L'Z', "after fflush() the read returned the discarded L'Z' (the pushed-back character must be discarded)");
	}
	pc_pass();
}
