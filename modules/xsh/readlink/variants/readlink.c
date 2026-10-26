/*
 * readlink.c - conforming variants of readlink() and readlinkat(), for the
 * variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "scribble": when the contents are shorter than bufsize, the rest of the
 * buffer (up to bufsize) is overwritten with 'Z'. XSH readlink()
 * DESCRIPTION leaves the remainder of buf unspecified in that case.
 */
static ssize_t after(ssize_t r, char *buf, size_t size)
{
	const char *v = variant_of("readlink.");

	if (r >= 0 && (size_t)r < size && IS("scribble"))
		memset(buf + r, 'Z', size - (size_t)r);
	return r;
}

ssize_t readlink(const char *restrict path, char *restrict buf, size_t size)
{
	REAL(readlink);
	return after(real_readlink(path, buf, size), buf, size);
}

ssize_t readlinkat(int fd, const char *restrict path, char *restrict buf, size_t size)
{
	REAL(readlinkat);
	return after(real_readlinkat(fd, path, buf, size), buf, size);
}
