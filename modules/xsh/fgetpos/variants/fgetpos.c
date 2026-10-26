/*
 * fgetpos.c - conforming variants of fgetpos() and fsetpos(), for the
 * variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "opaque": the fpos_t value is stored scrambled (every byte xor 0x5a) and
 * unscrambled again by fsetpos(). XSH fgetpos() DESCRIPTION leaves what
 * the stored value contains unspecified, asking only that fsetpos() can
 * use it to return to the position; its form is the implementation's.
 */
static void scramble(fpos_t *pos)
{
	unsigned char *b = (unsigned char *)pos;
	size_t i;

	for (i = 0; i < sizeof *pos; i++)
		b[i] ^= 0x5a;
}

int fgetpos(FILE *restrict f, fpos_t *restrict pos)
{
	const char *v = variant_of("fgetpos.");
	int r;
	REAL(fgetpos);

	r = real_fgetpos(f, pos);
	if (IS("opaque") && r == 0)
		scramble(pos);
	return r;
}

int fsetpos(FILE *f, const fpos_t *pos)
{
	const char *v = variant_of("fgetpos.");
	fpos_t p;
	REAL(fsetpos);

	if (!IS("opaque"))
		return real_fsetpos(f, pos);
	p = *pos;
	scramble(&p);
	return real_fsetpos(f, &p);
}
