/*
 * getdelim() enlarges an object that is too small and updates *n.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getdelim.004
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

int main(void)
{
	pc_begin("XSH.getdelim.004");
	{
		static char rec[102];
		FILE *f;
		char *p;
		size_t n = 2;
		ssize_t r;
		int i;

		memset(rec, 'x', 100);
		rec[100] = ':';
		rec[101] = '\0';
		pc_make_file("f", rec);
		p = malloc(2);
		pc_check(p != NULL, "malloc failed");
		pc_test();
		f = open_as("f", "r");
		r = getdelim(&p, &n, ':', f);
		pc_check(r == 101, "getdelim() returned %ld, expected 101", (long)r);
		pc_check(n >= 102, "*n is %lu after a 101-byte record, expected at least 102", (unsigned long)n);
		pc_check(p != NULL, "*lineptr is a null pointer");
		for (i = 0; i < 100; i++)
			pc_check(p[i] == 'x', "byte %d of the record is %#x, expected 'x'", i, (unsigned)(unsigned char)p[i]);
		pc_check(p[100] == ':' && p[101] == '\0', "the record does not end with ':' and a null byte");
		free(p);
	}
	pc_pass();
}
