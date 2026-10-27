/*
 * tmpfile() returns a stream on a new, empty regular file open for update.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tmpfile.001
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
	pc_begin("XSH.tmpfile.001");
	{
		static const unsigned char data[5] = { 'a', 'b', 0x00, 0xff, '\n' };
		unsigned char buf[8];
		struct stat st;
		FILE *f;
		long pos;
		int c;
		size_t n;

		pc_test();
		errno = 0;
		f = tmpfile();
		if (f == NULL && (errno == ENOSPC || errno == ENFILE || errno == ENOMEM))
			pc_unresolved("tmpfile() failed with %s: a resource of this environment is exhausted", pc_errname(errno));
		pc_check(f != NULL, "tmpfile() returned a null pointer: %s", pc_errname(errno));
		NEED(fileno(f) >= 0, "fileno() failed: %s", pc_errname(errno));
		NEED(fstat(fileno(f), &st) == 0, "fstat() failed: %s", pc_errname(errno));
		pc_check(S_ISREG(st.st_mode), "the temporary file is not a regular file (mode %#o)", (unsigned)st.st_mode);
		pc_check(st.st_size == 0, "the new temporary file has size %ld, expected 0", (long)st.st_size);
		pos = ftell(f);
		pc_check(pos == 0, "ftell() on the new stream is %ld, expected 0", pos);
		n = fwrite(data, 1, sizeof data, f);
		pc_check(n == sizeof data, "fwrite() of five bytes returned %lu", (unsigned long)n);
		NEED(fflush(f) == 0, "fflush() failed: %s", pc_errname(errno));
		rewind(f);
		memset(buf, 0, sizeof buf);
		n = fread(buf, 1, sizeof buf, f);
		pc_check(n == sizeof data && memcmp(buf, data, sizeof data) == 0, "reading back gave %lu bytes: %s, expected 61 62 00 ff 0a", (unsigned long)n, hex(buf, n));
		NEED(fseek(f, 0, SEEK_SET) == 0, "fseek() failed: %s", pc_errname(errno));
		c = fputc('Z', f);
		pc_check(c == 'Z', "fputc('Z') returned %d: the stream is not open for update", c);
		NEED(fseek(f, 0, SEEK_SET) == 0, "fseek() failed: %s", pc_errname(errno));
		c = getc(f);
		pc_check(c == 'Z', "getc() after overwriting the first byte returned %d, expected 'Z'", c);
		c = getc(f);
		pc_check(c == 'b', "the next getc() returned %d, expected 'b'", c);
		(void)fclose(f);
	}
	pc_pass();
}
