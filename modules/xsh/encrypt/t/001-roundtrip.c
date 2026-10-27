/*
 * Encoding a block and then decoding it with the same key gives the block back.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.encrypt.001
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
	char key[64], block[64], orig[64];
	unsigned s;

	pc_begin("XSH.encrypt.001");
	pc_test();
	for (s = 1; s <= 4; s++) {
		bits(key, s);
		bits(orig, s + 100);
		errno = 0;
		setkey(key);
		notsup();
		memcpy(block, orig, 64);
		errno = XS;
		encrypt(block, 0);
		notsup();
		pc_check(errno == XS, "encrypt(block, 0) changed errno to %s", pc_errname(errno));
		errno = XS;
		encrypt(block, 1);
		if (errno == ENOSYS)
			pc_notinuse("decoding is not provided (ENOSYS)");
		pc_check(errno == XS, "encrypt(block, 1) changed errno to %s", pc_errname(errno));
		pc_check(memcmp(block, orig, 64) == 0, "decoding with the key did not restore the block (set %u)", s);
	}
	pc_pass();
}
