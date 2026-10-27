/*
 * Bits of the key that the page says are ignored do not affect the encoding.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setkey.001
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
	char key[64], key2[64], key3[64], a[64], b[64], c[64], blk[64];
	int g;

	pc_begin("XSH.setkey.001");
	pc_test();
	bits(key, 3);
	bits(blk, 77);
	memcpy(key2, key, 64);
	memcpy(key3, key, 64);
	for (g = 0; g < 8; g++) {
		key2[8 * g + 7] = (char)(1 - key2[8 * g + 7]);
		key3[8 * g] = (char)(1 - key3[8 * g]);
	}
	errno = 0;
	setkey(key);
	notsup();
	memcpy(a, blk, 64);
	encrypt(a, 0);
	notsup();
	setkey(key2);
	memcpy(b, blk, 64);
	encrypt(b, 0);
	setkey(key3);
	memcpy(c, blk, 64);
	encrypt(c, 0);
	pc_check(memcmp(a, b, 64) == 0 || memcmp(a, c, 64) == 0,
	    "changing neither the first nor the last element of each group of 8 left the encoding unchanged");
	pc_pass();
}
