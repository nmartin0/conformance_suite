/*
 * A tmpfile() file lives while another reference to it is open.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tmpfile.002
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
	pc_begin("XSH.tmpfile.002");
	{
		char buf[8];
		FILE *f;
		int fd2;
		ssize_t r;

		pc_test();
		f = tmpfile();
		NEED(f != NULL, "tmpfile() returned a null pointer: %s", pc_errname(errno));
		NEED(fputs("hello", f) >= 0, "fputs() failed: %s", pc_errname(errno));
		NEED(fflush(f) == 0, "fflush() failed: %s", pc_errname(errno));
		fd2 = dup(fileno(f));
		NEED(fd2 >= 0, "dup() failed: %s", pc_errname(errno));
		NEED(fclose(f) == 0, "fclose() failed: %s", pc_errname(errno));
		memset(buf, 0, sizeof buf);
		r = pread(fd2, buf, 5, 0);
		pc_check(r == 5 && memcmp(buf, "hello", 5) == 0, "pread() from the duplicate descriptor after fclose() returned %ld: the file did not outlive the stream while a reference was open", (long)r);
		(void)close(fd2);
	}
	pc_pass();
}
