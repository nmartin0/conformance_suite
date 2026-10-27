/*
 * An encoded block still holds only the values 0 and 1, and errno is left alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.encrypt.002
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define XS EDOM

static void bits(char *a, unsigned seed)
{
	int i;

	for (i = 0; i < 64; i++)
		a[i] = (char)(((seed * 2654435761u) >> (i % 29) ^ (unsigned)i * 7u) & 1u);
}

static void notsup(void)
{
	if (errno == ENOSYS)
		pc_notinuse("the function is not supported (ENOSYS)");
}

int main(void)
{
	char key[64], block[64];
	int i, v;

	pc_begin("XSH.encrypt.002");
	pc_test();
	bits(key, 9);
	errno = 0;
	setkey(key);
	notsup();
	for (v = 0; v < 3; v++) {
		for (i = 0; i < 64; i++)
			block[i] = (char)(v == 0 ? 0 : v == 1 ? 1 : (i * 5 + 1) % 2);
		errno = EDOM;
		encrypt(block, 0);
		notsup();
		pc_check(errno == EDOM, "encrypt() changed errno to %s", pc_errname(errno));
		for (i = 0; i < 64; i++)
			pc_check(block[i] == 0 || block[i] == 1, "element %d is %d after encoding", i, (int)block[i]);
	}
	pc_pass();
}
