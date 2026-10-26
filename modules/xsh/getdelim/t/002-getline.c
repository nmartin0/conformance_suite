/*
 * getline() reads newline-delimited records.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdelim.002
 * MUTANT    getdelim.getline_drops_delim
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
	pc_begin("XSH.getdelim.002");
	{
		FILE *f;
		char *p = NULL;
		size_t n = 0;
		ssize_t r;

		pc_make_file("f", "one\ntwo");
		pc_test();
		f = open_as("f", "r");
		r = getline(&p, &n, f);
		pc_check(r == 4, "the first getline() returned %ld, expected 4", (long)r);
		pc_check(p != NULL && memcmp(p, "one\n", 5) == 0, "the object holds %s, expected 6f 6e 65 0a 00", p ? hex(p, 5) : "nothing");
		r = getline(&p, &n, f);
		pc_check(r == 3, "the second getline() returned %ld, expected 3", (long)r);
		pc_check(memcmp(p, "two", 4) == 0, "the object holds %s, expected 74 77 6f 00", hex(p, 4));
		r = getline(&p, &n, f);
		pc_check(r == -1, "getline() at end of file returned %ld, expected -1", (long)r);
		pc_check(feof(f) != 0, "feof() is false after getline() returned -1 at end of file");
		free(p);
	}
	pc_pass();
}
