/*
 * A successful fscanf() marks the file's access time for update.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.049
 * MUTANT    fscanf.no_atime
 * VARIANT   setvbuf.tiny_default
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

int main(void)
{
	struct timespec ts[2];
	struct stat st;
	FILE *f;
	int x = -1, r;

	pc_begin("XSH.fscanf.049");
	pc_make_file("f", "123");
	pc_check(stat("f", &st) == 0, "stat failed");
	ts[0].tv_sec = st.st_mtim.tv_sec - 100;
	ts[0].tv_nsec = 0;
	ts[1].tv_sec = 0;
	ts[1].tv_nsec = UTIME_OMIT;
	pc_check(utimensat(AT_FDCWD, "f", ts, 0) == 0, "cannot set the access time back");
	f = fopen("f", "r");
	pc_check(f != NULL, "fopen(f) failed: %s", pc_errname(errno));
	pc_test();
	r = fscanf(f, "%d", &x);
	pc_check(r == 1 && x == 123, "fscanf() returned %d and %d", r, x);
	NEED(fstat(fileno(f), &st) == 0, "fstat failed: %s", pc_errname(errno));
	pc_check(st.st_atim.tv_sec > ts[0].tv_sec,
	    "the access time was not updated (is the file system mounted noatime or atime=off?)");
	pc_pass();
}
