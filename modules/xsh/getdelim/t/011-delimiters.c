/*
 * getdelim() takes any unsigned char value as delimiter, 0 and 0xff included.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdelim.011
 * MUTANT    getdelim.signed_delim
 * VARIANT   setvbuf.tiny_default
 * VARIANT   fopen.read_all
 * VARIANT   getdelim.n_exact
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

/* A file holding exactly the len bytes at data (setup). */
static void make_bytes(const char *path, const void *data, size_t len)
{
	int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	pc_check(len == 0 || write(fd, data, len) == (ssize_t)len, "write(%s) failed: %s", path, pc_errname(errno));
	pc_check(close(fd) == 0, "close(%s) failed: %s", path, pc_errname(errno));
}

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
	pc_begin("XSH.getdelim.011");
	{
		static const char data[] = { 'a', 'b', 0, 'c', 'd', (char)0xff, 'e', 'f' };
		FILE *f;
		char *p = NULL;
		size_t n = 0;
		ssize_t r;

		make_bytes("f", data, sizeof data);
		pc_test();
		f = open_as("f", "r");
		r = getdelim(&p, &n, 0, f);
		pc_check(r == 3, "getdelim() with delimiter 0 returned %ld, expected 3", (long)r);
		pc_check(p != NULL && memcmp(p, "ab\0", 4) == 0, "the object holds %s, expected 61 62 00 00", p ? hex(p, 4) : "nothing");
		r = getdelim(&p, &n, 0xff, f);
		pc_check(r == 3, "getdelim() with delimiter 0xff returned %ld, expected 3", (long)r);
		pc_check(memcmp(p, "cd\xff", 4) == 0, "the object holds %s, expected 63 64 ff 00", hex(p, 4));
		r = getdelim(&p, &n, 0xff, f);
		pc_check(r == 2, "the last getdelim() returned %ld, expected 2", (long)r);
		pc_check(memcmp(p, "ef", 3) == 0, "the object holds %s, expected 65 66 00", hex(p, 3));
		free(p);
	}
	pc_pass();
}
