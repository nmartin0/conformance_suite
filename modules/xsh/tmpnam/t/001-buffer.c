/*
 * tmpnam() with a caller's buffer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tmpnam.001
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int main(void)
{
	char buf[L_tmpnam + 16];
	char *r;
	struct stat sb;
	int i;

	pc_begin("XSH.tmpnam.001");
	pc_test();
	for (i = 0; i < 5; i++) {
		memset(buf, 0, sizeof buf);
		r = tmpnam(buf);
		pc_check(r != NULL, "tmpnam() returned a null pointer");
		pc_check(r == buf, "tmpnam(buf) returned %p, not the argument %p", (void *)r, (void *)buf);
		pc_check(buf[0] != '\0', "the name is empty");
		pc_check(strlen(buf) < L_tmpnam, "the name has %lu characters, not fewer than L_tmpnam (%d)", (unsigned long)strlen(buf), (int)L_tmpnam);
		errno = 0;
		pc_check(lstat(buf, &sb) == -1 && errno == ENOENT, "the name \"%s\" names an existing file", buf);
	}
	pc_pass();
}
