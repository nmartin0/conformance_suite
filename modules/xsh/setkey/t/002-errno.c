/*
 * setkey() leaves errno alone when it succeeds.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setkey.002
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
	char key[64];

	pc_begin("XSH.setkey.002");
	pc_test();
	bits(key, 5);
	errno = EDOM;
	setkey(key);
	notsup();
	pc_check(errno == EDOM, "setkey() changed errno to %s", pc_errname(errno));
	pc_pass();
}
