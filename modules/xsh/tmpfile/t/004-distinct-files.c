/*
 * Two tmpfile() calls create two independent files.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tmpfile.004
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

int main(void)
{
	pc_begin("XSH.tmpfile.004");
	{
		struct stat s1, s2;
		char b1[8], b2[8];
		FILE *f1, *f2;
		size_t n1, n2;

		pc_test();
		f1 = tmpfile();
		NEED(f1 != NULL, "the first tmpfile() returned a null pointer: %s", pc_errname(errno));
		f2 = tmpfile();
		NEED(f2 != NULL, "the second tmpfile() returned a null pointer: %s", pc_errname(errno));
		pc_check(f1 != f2, "both calls returned the same stream");
		NEED(fstat(fileno(f1), &s1) == 0 && fstat(fileno(f2), &s2) == 0, "fstat() failed: %s", pc_errname(errno));
		pc_check(!(s1.st_dev == s2.st_dev && s1.st_ino == s2.st_ino), "both streams are on the same file");
		NEED(fputs("one", f1) >= 0 && fputs("two", f2) >= 0, "fputs() failed: %s", pc_errname(errno));
		NEED(fflush(f1) == 0 && fflush(f2) == 0, "fflush() failed: %s", pc_errname(errno));
		rewind(f1);
		rewind(f2);
		memset(b1, 0, sizeof b1);
		memset(b2, 0, sizeof b2);
		n1 = fread(b1, 1, sizeof b1, f1);
		n2 = fread(b2, 1, sizeof b2, f2);
		pc_check(n1 == 3 && memcmp(b1, "one", 3) == 0, "the first file reads back %lu bytes \"%s\", expected \"one\"", (unsigned long)n1, b1);
		pc_check(n2 == 3 && memcmp(b2, "two", 3) == 0, "the second file reads back %lu bytes \"%s\", expected \"two\"", (unsigned long)n2, b2);
		(void)fclose(f1);
		(void)fclose(f2);
	}
	pc_pass();
}
