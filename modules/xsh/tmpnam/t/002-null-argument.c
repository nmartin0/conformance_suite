/*
 * tmpnam() with a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tmpnam.002
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int main(void)
{
	char *r;
	struct stat sb;
	int i;

	pc_begin("XSH.tmpnam.002");
	pc_test();
	for (i = 0; i < 5; i++) {
		r = tmpnam(NULL);
		pc_check(r != NULL, "tmpnam(NULL) returned a null pointer");
		pc_check(r[0] != '\0', "the name is empty");
		errno = 0;
		pc_check(lstat(r, &sb) == -1 && errno == ENOENT, "the name \"%s\" names an existing file", r);
	}
	pc_pass();
}
