/*
 * setvbuf.c - conforming variants of stream buffering, for the variant
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it. These model choices every stdio test can depend on
 * (how large a buffer is, whether a caller's array is used), so tests of
 * other functions in this group declare them too.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * Some C libraries keep the first 8 bytes of an array given to setvbuf() for pushback and ignore a smaller array; others use all of it. SLACK is added so that the usable buffer has the size meant here on both.
 */
#ifdef __GLIBC__
#define SLACK 0
#else
#define SLACK 8
#endif

/* The usable size of a tiny default buffer. */
#define TINY 2

/*
 * "tiny_default": a stream that is opened on something other than a
 * terminal is fully buffered, but its buffer holds only TINY bytes, so
 * output reaches the file a byte or two at a time and input is read ahead
 * by as little. XSH 2.5 lets bytes be accumulated without fixing how many,
 * and fopen() asks for full buffering without fixing a size; nothing sets a
 * minimum buffer size. An array the application gives to setvbuf() is used;
 * a size given with a null buffer is ignored (setvbuf: the size "may"
 * determine the buffer), so such a stream keeps its tiny buffer.
 */
static FILE *tiny(FILE *f)
{
	REAL(setvbuf);
	int e = errno, fd;
	char *b;

	if (f == NULL)
		return f;
	fd = fileno(f);
	if (fd >= 0 && isatty(fd)) {
		errno = e;
		return f;
	}
	b = malloc(TINY + SLACK);
	if (b != NULL)
		(void)real_setvbuf(f, b, _IOFBF, TINY + SLACK);
	errno = e;
	return f;
}

FILE *fopen(const char *restrict path, const char *restrict mode)
{
	const char *v = variant_of("setvbuf.");
	REAL(fopen);
	FILE *f = real_fopen(path, mode);

	return IS("tiny_default") ? tiny(f) : f;
}

FILE *fdopen(int fd, const char *mode)
{
	const char *v = variant_of("setvbuf.");
	REAL(fdopen);
	FILE *f = real_fdopen(fd, mode);

	return IS("tiny_default") ? tiny(f) : f;
}

FILE *freopen(const char *restrict path, const char *restrict mode, FILE *restrict stream)
{
	const char *v = variant_of("setvbuf.");
	REAL(freopen);
	FILE *f = real_freopen(path, mode, stream);

	return IS("tiny_default") ? tiny(f) : f;
}

FILE *fmemopen(void *restrict buf, size_t size, const char *restrict mode)
{
	const char *v = variant_of("setvbuf.");
	REAL(fmemopen);
	FILE *f = real_fmemopen(buf, size, mode);

	return IS("tiny_default") ? tiny(f) : f;
}

int setvbuf(FILE *restrict f, char *restrict buf, int type, size_t size)
{
	const char *v = variant_of("setvbuf.");
	REAL(setvbuf);
	int fd;

	if (v == NULL || (type != _IOFBF && type != _IOLBF && type != _IONBF))
		return real_setvbuf(f, buf, type, size);
	/*
	 * "size_honored": with a null buffer, the size given decides the size
	 * of the buffer setvbuf() allocates. setvbuf() says the size may
	 * determine it; the reference systems ignore it.
	 */
	if (IS("size_honored") && buf == NULL && type != _IONBF && size > 0) {
		char *b = malloc(size + SLACK);

		if (b != NULL)
			return real_setvbuf(f, b, type, size + SLACK);
	}
	/*
	 * "own_unused": a caller's array is not used; the stream gets a buffer
	 * of the same size of its own. setvbuf() says the array may be used,
	 * not that it shall be.
	 */
	if (IS("own_unused") && buf != NULL && type != _IONBF) {
		char *b = malloc(size);

		if (b != NULL)
			return real_setvbuf(f, b, type, size);
	}
	/*
	 * "scribbles": the caller's array is used, and its whole contents are
	 * changed at once. setvbuf() leaves the contents of the array
	 * unspecified at any time.
	 */
	if (IS("scribbles") && buf != NULL && size > 0)
		memset(buf, 0xa5, size);
	/*
	 * "refuses_modes": only full buffering is provided for streams that
	 * are not terminals; a request for line buffering or no buffering on
	 * one fails (non-zero, errno untouched). XSH 2.5 makes support for the
	 * buffering characteristics implementation-defined, and setvbuf()
	 * returns non-zero when a request cannot be honoured (it may set errno,
	 * so leaving it alone is allowed). Its APPLICATION USAGE notes that
	 * many systems provide line buffering only for terminals.
	 */
	if (IS("refuses_modes") && type != _IOFBF) {
		int e = errno;

		fd = fileno(f);
		if (!(fd >= 0 && isatty(fd))) {
			errno = e;
			return -1;
		}
		errno = e;
	}
	return real_setvbuf(f, buf, type, size);
}
