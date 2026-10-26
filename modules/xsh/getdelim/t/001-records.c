/*
 * getdelim() allocates, reads each record with its delimiter, and ends with -1.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdelim.001
 * MUTANT    getdelim.drops_delim
 * MUTANT    getdelim.no_null
 * MUTANT    getdelim.n_not_set
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
	pc_begin("XSH.getdelim.001");
	{
		FILE *f;
		char *p = NULL;
		size_t n = 0;
		ssize_t r;

		pc_make_file("f", "ab:cd:ef");
		pc_test();
		f = open_as("f", "r");
		r = getdelim(&p, &n, ':', f);
		pc_check(r == 3, "the first getdelim() returned %ld, expected 3", (long)r);
		pc_check(p != NULL && memcmp(p, "ab:", 4) == 0, "the object holds %s, expected 61 62 3a 00", p ? hex(p, 4) : "nothing");
		pc_check(n >= 4, "*n is %lu after a 3-byte record, expected at least 4", (unsigned long)n);
		r = getdelim(&p, &n, ':', f);
		pc_check(r == 3, "the second getdelim() returned %ld, expected 3", (long)r);
		pc_check(memcmp(p, "cd:", 4) == 0, "the object holds %s, expected 63 64 3a 00", hex(p, 4));
		pc_check(n >= 4, "*n is %lu after a 3-byte record, expected at least 4", (unsigned long)n);
		r = getdelim(&p, &n, ':', f);
		pc_check(r == 2, "the third getdelim() returned %ld, expected 2", (long)r);
		pc_check(memcmp(p, "ef", 3) == 0, "the object holds %s, expected 65 66 00", hex(p, 3));
		r = getdelim(&p, &n, ':', f);
		pc_check(r == -1, "getdelim() at end of file returned %ld, expected -1", (long)r);
		pc_check(feof(f) != 0, "feof() is false after getdelim() returned -1 at end of file");
		free(p);
	}
	pc_pass();
}
