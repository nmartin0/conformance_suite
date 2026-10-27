/*
 * getentropy() with permitted lengths.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getentropy.001
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	static const size_t lens[] = { 0, 1, 16, 64, 256, GETENTROPY_MAX };
	size_t i, j, cap = GETENTROPY_MAX + 8;
	unsigned char *buf;

	pc_begin("XSH.getentropy.001");
	buf = malloc(cap);
	pc_check(buf != NULL, "malloc failed");
	pc_test();
	for (i = 0; i < sizeof lens / sizeof lens[0]; i++) {
		int r;

		memset(buf, 0xA5, cap);
		r = getentropy(buf, lens[i]);
		if (r == -1 && errno == ENOSYS)
			pc_notinuse("getentropy() is not provided (ENOSYS)");
		pc_check(r == 0, "getentropy(length %lu) returned %d (errno %s)", (unsigned long)lens[i], r, pc_errname(errno));
		for (j = lens[i]; j < cap; j++)
			pc_check(buf[j] == 0xA5, "byte %lu beyond the length %lu was modified", (unsigned long)j, (unsigned long)lens[i]);
	}
	free(buf);
	pc_pass();
}
