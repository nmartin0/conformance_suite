/*
 * Multibyte characters count once and %s converts them back to bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fwscanf.018
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

/* The file's whole contents (at most size - 1 bytes) and its length. */
static long slurp(const char *path, char *buf, size_t size)
{
	int fd = open(path, O_RDONLY);
	long n = 0, r;

	NEED(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	while ((size_t)n < size - 1 && (r = read(fd, buf + n, size - 1 - (size_t)n)) > 0)
		n += r;
	(void)close(fd);
	buf[n] = '\0';
	return n;
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
	pc_begin("XSH.fwscanf.018");
	{
		const char bytes[3] = { (char)0xc3, (char)0xa9, 'x' };
		wchar_t in[3], ws[8], w;
		char cs[8];
		FILE *f;
		int r, n;

		utf8();
		in[0] = e_acute();
		in[1] = L'x';
		in[2] = L'\0';
		make_bytes("f", bytes, sizeof bytes);
		pc_test();
		memset(cs, '#', sizeof cs);
		r = swscanf(in, L"%s", cs);
		pc_check(r == 1 && memcmp(cs, "\xc3\xa9x", 4) == 0, "swscanf(%%s) returned %d and stored %s, expected 1 and c3 a9 78 00", r, hex(cs, 4));
		wmemset(ws, L'#', 8);
		n = -1;
		r = swscanf(in, L"%ls%n", ws, &n);
		pc_check(r == 1 && ws[0] == in[0] && ws[1] == L'x' && ws[2] == L'\0' && n == 2, "swscanf(%%ls%%n) returned %d with n = %d, expected 1 and 2 wide characters", r, n);
		w = L'#';
		r = swscanf(in, L"%lc", &w);
		pc_check(r == 1 && w == in[0], "swscanf(%%lc) returned %d, expected 1 and the first character", r);
		f = open_as("f", "r");
		n = -1;
		wmemset(ws, L'#', 8);
		r = fwscanf(f, L"%ls%n", ws, &n);
		pc_check(r == 1 && ws[0] == in[0] && ws[1] == L'x' && n == 2, "fwscanf(%%ls%%n) on the three bytes returned %d with n = %d, expected 1 and 2 wide characters", r, n);
	}
	pc_pass();
}
