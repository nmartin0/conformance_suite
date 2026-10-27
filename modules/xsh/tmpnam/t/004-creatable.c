/*
 * A tmpnam() name is a valid pathname in an existing directory.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tmpnam.004
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int main(void)
{
	char buf[L_tmpnam + 16], dir[L_tmpnam + 16];
	struct stat sb;
	char *slash;
	int i;

	pc_begin("XSH.tmpnam.004");
	pc_test();
	for (i = 0; i < 3; i++) {
		pc_check(tmpnam(buf) == buf, "tmpnam() failed");
		errno = 0;
		pc_check(lstat(buf, &sb) == -1 && errno == ENOENT, "the name \"%s\" is not a valid pathname of a missing file (errno %s)",
		    buf, pc_errname(errno));
		/* The directory part of the name exists, so the file could be created there. */
		slash = strrchr(buf, '/');
		if (slash != NULL) {
			size_t n = (size_t)(slash - buf);

			memcpy(dir, buf, n);
			dir[n] = '\0';
			if (n == 0)
				strcpy(dir, "/");
			pc_check(stat(dir, &sb) == 0 && S_ISDIR(sb.st_mode), "the directory part \"%s\" of \"%s\" is not a directory", dir, buf);
		}
	}
	pc_pass();
}
