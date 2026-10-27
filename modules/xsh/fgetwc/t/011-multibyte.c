/*
 * fgetwc() converts a multibyte character as mbrtowc() does.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fgetwc.011
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

/* A file holding exactly the len bytes at data (setup). */
static void make_bytes(const char *path, const void *data, size_t len)
{
	int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	pc_check(len == 0 || write(fd, data, len) == (ssize_t)len, "write(%s) failed: %s", path, pc_errname(errno));
	pc_check(close(fd) == 0, "close(%s) failed: %s", path, pc_errname(errno));
}

/* Setup: switches to an installed UTF-8 locale, or the test is not in use. */
static void utf8(void)
{
	const char *name = pc_find_locale("UTF-8", 1);

	if (name == NULL)
		pc_notinuse("no UTF-8 locale installed");
	if (setlocale(LC_ALL, name) == NULL)
		pc_unresolved("setlocale(%s) failed", name);
}


/* Setup: the wide character mbrtowc() gives for the two bytes 0xc3 0xa9. */
static wchar_t e_acute(void)
{
	mbstate_t st;
	wchar_t w = 0;

	memset(&st, 0, sizeof st);
	if (!(mbrtowc(&w, "\xc3\xa9", 2, &st) == 2))
		pc_unresolved("mbrtowc() does not convert the bytes c3 a9 to one character");
	return w;
}

int main(void)
{
	pc_begin("XSH.fgetwc.011");
	{
		const char bytes[3] = { (char)0xc3, (char)0xa9, 'x' };
		FILE *f;
		wint_t c;
		wchar_t want;
		long pos;

		utf8();
		want = e_acute();
		make_bytes("f", bytes, sizeof bytes);
		pc_test();
		f = open_as("f", "r");
		c = fgetwc(f);
		pc_check(c == (wint_t)want, "fgetwc() returned %#lx for the bytes c3 a9, expected %#lx (as mbrtowc() converts them)", (unsigned long)c, (unsigned long)want);
		pos = ftell(f);
		NEED(pos != -1, "ftell() failed: %s", pc_errname(errno));
		pc_check(pos == 2, "ftell() after one two-byte character is %ld, expected 2", pos);
		c = fgetwc(f);
		pc_check(c == L'x', "the second fgetwc() returned %#lx, expected L'x'", (unsigned long)c);
	}
	pc_pass();
}
