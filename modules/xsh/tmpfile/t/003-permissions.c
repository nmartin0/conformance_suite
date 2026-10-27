/*
 * The file tmpfile() creates has the permissions of fopen() or a permitted restriction.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tmpfile.003
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
	pc_begin("XSH.tmpfile.003");
	{
		static const mode_t masks[2] = { 0, 022 };
		static const unsigned full[2] = { 0666, 0644 };
		struct stat st;
		FILE *f;
		unsigned m;
		int i;

		pc_test();
		for (i = 0; i < 2; i++) {
			(void)umask(masks[i]);
			f = tmpfile();
			NEED(f != NULL, "tmpfile() returned a null pointer: %s", pc_errname(errno));
			NEED(fstat(fileno(f), &st) == 0, "fstat() failed: %s", pc_errname(errno));
			m = (unsigned)(st.st_mode & 0777);
			pc_check(m == full[i] || m == 0600 || m == 0, "with umask %#o the permission bits are %#o, expected %#o, 0600 or 0", (unsigned)masks[i], m, full[i]);
			(void)fclose(f);
		}
	}
	pc_pass();
}
