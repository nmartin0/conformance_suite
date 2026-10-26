/*
 * fgetc() returns each byte as an unsigned char value and advances the position.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fgetc.001
 * MUTANT    fgetc.sign_extends
 * MUTANT    fgetc.no_advance
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

int main(void)
{
	pc_begin("XSH.fgetc.001");
	{
		static const unsigned char want[] = { 0x41, 0xff, 0x00, 0x42 };
		FILE *f;
		int i, c;
		long pos;

		make_bytes("f", want, sizeof want);
		pc_test();
		f = open_as("f", "r");
		for (i = 0; i < 4; i++) {
			c = fgetc(f);
			pc_check(c == want[i], "fgetc() call %d returned %d, expected %d", i + 1, c, want[i]);
			pos = ftell(f);
			NEED(pos != -1, "ftell() failed: %s", pc_errname(errno));
			pc_check(pos == i + 1, "ftell() after fgetc() call %d is %ld, expected %d", i + 1, pos, i + 1);
		}
	}
	pc_pass();
}
