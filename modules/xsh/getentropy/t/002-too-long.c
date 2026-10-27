/*
 * getentropy() rejects a length above {GETENTROPY_MAX}.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getentropy.002
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	size_t cap = (size_t)GETENTROPY_MAX + 2;
	unsigned char *buf;
	int r;

	pc_begin("XSH.getentropy.002");
	buf = malloc(cap);
	pc_check(buf != NULL, "malloc failed");
	pc_test();
	errno = 0;
	r = getentropy(buf, (size_t)GETENTROPY_MAX + 1);
	pc_check(r == -1, "getentropy(GETENTROPY_MAX + 1) returned %d", r);
	if (errno == ENOSYS)
		pc_notinuse("getentropy() is not provided (ENOSYS)");
	pc_check(errno == EINVAL, "errno is %s, expected EINVAL", pc_errname(errno));
	/* A length this large must be refused before any byte is written. */
	errno = 0;
	r = getentropy(buf, SIZE_MAX / 2);
	pc_check(r == -1, "getentropy(SIZE_MAX / 2) returned %d", r);
	pc_check(errno == EINVAL, "errno is %s, expected EINVAL", pc_errname(errno));
	free(buf);
	pc_pass();
}
