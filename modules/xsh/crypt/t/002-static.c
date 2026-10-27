/*
 * The result of crypt() is overwritten by the next call.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.crypt.002
 */
#include "pc.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	char *r1, *r2, first[512];

	pc_begin("XSH.crypt.002");
	pc_test();
	errno = 0;
	r1 = crypt("first key", "ab");
	if (r1 == NULL) {
		if (errno == ENOSYS)
			pc_notinuse("crypt() is not supported (ENOSYS)");
		pc_unresolved("crypt() returned a null pointer (errno %s)", pc_errname(errno));
	}
	pc_check(strlen(r1) < sizeof first, "the result is unexpectedly long");
	strcpy(first, r1);
	r2 = crypt("second key", "cd");
	pc_check(r2 != NULL, "the second crypt() call failed");
	pc_check(strncmp(r2, "cd", 2) == 0 && strncmp(first, "ab", 2) == 0, "the results do not begin with their salts");
	pc_check(strcmp(r1, r2) == 0, "the first result still holds its own value after the second call");
	pc_pass();
}
