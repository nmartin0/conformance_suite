/*
 * fgets() reads at most n-1 bytes and leaves the rest of the line.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fgets.002
 * MUTANT    fgets.n_bytes
 * MUTANT    fgets.skips_rest
 * VARIANT   setvbuf.tiny_default
 * VARIANT   fopen.read_all
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

/* A stream on path in mode (a precondition). */
static FILE *open_as(const char *path, const char *mode)
{
	FILE *f = fopen(path, mode);

	NEED(f != NULL, "fopen(%s, %s) failed: %s", path, mode, pc_errname(errno));
	return f;
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
	pc_begin("XSH.fgets.002");
	{
		FILE *f;
		char s[16], *p;
		int c;

		pc_make_file("f", "abcdefgh\n");
		pc_test();
		f = open_as("f", "r");
		memset(s, 'Q', sizeof s);
		p = fgets(s, 4, f);
		pc_check(p == s, "fgets(s, 4, f) returned %p, expected s (%p)", (void *)p, (void *)s);
		pc_check(memcmp(s, "abc", 4) == 0, "s holds %s, expected 61 62 63 00", hex(s, 4));
		c = fgetc(f);
		pc_check(c == 'd', "the read after fgets() returned %d, expected 'd'", c);
	}
	pc_pass();
}
