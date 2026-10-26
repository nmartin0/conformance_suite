/*
 * open_memstream.c - conforming variants of open_memstream() and
 * open_wmemstream(), for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it. Three of them replace open_memstream() with a stream
 * of this library's own, built on fopencookie() (a CI-only facility of the
 * reference C libraries), that follows the open_memstream() page and makes
 * one of the implementation-defined choices.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <wchar.h>
#include "pcvariant.h"

/*
 * "seek_extends": a seek past the current length extends the buffer to
 * the new position, filling it with null bytes, so the length becomes the
 * position. "seek_keeps": such a seek does not extend the buffer; a later
 * write fills the gap with null bytes. The open_memstream() page makes the
 * choice implementation-defined and fixes the null bytes either way.
 *
 * "seek_end_flush_size": SEEK_END is relative to the size an fflush()
 * would set at that moment (the smaller of the length and the position)
 * instead of the length; the page makes this choice implementation-defined
 * too. This stream does not extend on seeks.
 *
 * In all three the pointer and size are kept current after every write and
 * seek, which satisfies the rule that they are right after a successful
 * fflush() or fclose() (the page says nothing of them before).
 */
struct ms {
	int extend, end_flush, used;
	char **bufp;
	size_t *sizep;
	char *buf;
	size_t cap, pos, len;
};

/* Streams watched by the READINGs below. */
static struct {
	FILE *f;
	struct ms *c;		/* own byte stream, or NULL for a real wide one */
	int used;
} fresh[64];

static void publish(struct ms *c)
{
	*c->bufp = c->buf;
	*c->sizep = c->pos < c->len ? c->pos : c->len;
}

static int grow(struct ms *c, size_t need)
{
	size_t cap = c->cap;
	char *nb;

	if (need + 1 <= cap)
		return 0;
	while (cap < need + 1)
		cap = cap * 2 + 16;
	nb = realloc(c->buf, cap);
	if (nb == NULL)
		return -1;
	memset(nb + c->cap, 0, cap - c->cap);
	c->buf = nb;
	c->cap = cap;
	return 0;
}

static ssize_t mwrite(void *ck, const char *in, size_t n)
{
	struct ms *c = ck;

	c->used = 1;
	if (grow(c, c->pos + n) != 0)
		return -1;
	if (c->pos > c->len)
		memset(c->buf + c->len, 0, c->pos - c->len);
	memcpy(c->buf + c->pos, in, n);
	c->pos += n;
	if (c->pos > c->len) {
		c->len = c->pos;
		c->buf[c->len] = 0;
	}
	publish(c);
	return (ssize_t)n;
}

static int mseek(void *ck, off_t *off, int whence)
{
	struct ms *c = ck;
	long long base, np;

	if (whence == SEEK_SET)
		base = 0;
	else if (whence == SEEK_CUR)
		base = (long long)c->pos;
	else if (whence == SEEK_END)
		base = (long long)(c->end_flush && c->pos < c->len ? c->pos : c->len);
	else {
		errno = EINVAL;
		return -1;
	}
	np = base + (long long)*off;
	if (np < 0) {
		errno = EINVAL;
		return -1;
	}
	if (c->extend && np > (long long)c->len) {
		if (grow(c, (size_t)np) != 0)
			return -1;
		memset(c->buf + c->len, 0, (size_t)np - c->len + 1);
		c->len = (size_t)np;
	}
	c->pos = (size_t)np;
	publish(c);
	*off = (off_t)np;
	return 0;
}

static int mclose(void *ck)
{
	struct ms *c = ck;
	int i;

	publish(c);
	for (i = 0; i < 64; i++)
		if (fresh[i].c == c)
			fresh[i].f = NULL;
	free(c);
	return 0;
}

static struct ms *last_made;

static FILE *own_stream(char **bufp, size_t *sizep, int extend, int end_flush, int orient)
{
	cookie_io_functions_t io = { NULL, mwrite, mseek, mclose };
	struct ms *c = calloc(1, sizeof *c);
	FILE *f;

	if (c == NULL)
		return NULL;
	c->extend = extend;
	c->end_flush = end_flush;
	c->bufp = bufp;
	c->sizep = sizep;
	if (grow(c, 0) != 0) {
		free(c);
		return NULL;
	}
	f = fopencookie(c, "w", io);
	if (f == NULL) {
		free(c->buf);
		free(c);
		return NULL;
	}
	if (orient)
		(void)fwide(f, -1);	/* byte-oriented, as the page says */
	publish(c);
	last_made = c;
	return f;
}

/*
 * "unbuffered": memory streams start unbuffered, so every write reaches
 * the memory buffer at once. Neither the open_memstream() page nor XSH 2.5
 * fixes the initial buffering of a memory stream. The byte stream is this
 * library's own (not extending on seeks, SEEK_END from the length), made
 * unbuffered; a reference library's own open_memstream() stream crashes
 * after setvbuf(_IONBF), so it cannot carry this variant.
 *
 * "unoriented" and "wunoriented" (READING of XSH.open_memstream.012 and
 * .014): the stream's orientation is decided as for a file stream (XSH
 * 2.5), so it has none until the first input or output, and then the
 * one that output gives. "unoriented" uses this library's own byte stream,
 * not oriented at open; some fopencookie() streams are byte-oriented from
 * creation, so until the stream has been written (or oriented by fwide())
 * fwide(f, 0) answers 0 here. "wunoriented" keeps the real wide stream,
 * which is wide-oriented at open, and answers 0 until a wide output
 * function (fputwc(), putwc(), fputws(), fwprintf(), vfwprintf()) or
 * fwide() with a nonzero mode has been used on it.
 */

static void watch(FILE *f, struct ms *c)
{
	int i;

	for (i = 0; f != NULL && i < 64; i++)
		if (fresh[i].f == NULL) {
			fresh[i].f = f, fresh[i].c = c, fresh[i].used = 0;
			return;
		}
}

static int fresh_slot(FILE *f)
{
	int i;

	for (i = 0; f != NULL && i < 64; i++)
		if (fresh[i].f == f)
			return i;
	return -1;
}

static void wide_used(FILE *f)
{
	int i = fresh_slot(f);

	if (i >= 0)
		fresh[i].used = 1;
}

FILE *open_memstream(char **bufp, size_t *sizep)
{
	const char *v = variant_of("open_memstream.");
	REAL(open_memstream);
	REAL(setvbuf);
	FILE *f;

	if (IS("seek_extends") || IS("seek_keeps") || IS("seek_end_flush_size"))
		return own_stream(bufp, sizep, IS("seek_extends"), IS("seek_end_flush_size"), 1);
	if (IS("unoriented")) {
		f = own_stream(bufp, sizep, 0, 0, 0);
		watch(f, last_made);
		return f;
	}
	if (IS("unbuffered")) {
		f = own_stream(bufp, sizep, 0, 0, 1);
		if (f != NULL)
			(void)real_setvbuf(f, NULL, _IONBF, 0);
		return f;
	}
	return real_open_memstream(bufp, sizep);
}

FILE *open_wmemstream(wchar_t **bufp, size_t *sizep)
{
	const char *v = variant_of("open_memstream.");
	REAL(open_wmemstream);
	REAL(setvbuf);
	FILE *f = real_open_wmemstream(bufp, sizep);

	if (IS("unbuffered") && f != NULL)
		(void)real_setvbuf(f, NULL, _IONBF, 0);
	if (IS("wunoriented"))
		watch(f, NULL);
	return f;
}

int fwide(FILE *f, int mode)
{
	const char *v = variant_of("open_memstream.");
	REAL(fwide);
	int i = v != NULL ? fresh_slot(f) : -1;

	if (i >= 0) {
		if (mode != 0)
			fresh[i].used = 1;
		else if (!(fresh[i].c != NULL ? fresh[i].c->used : fresh[i].used))
			return 0;
	}
	return real_fwide(f, mode);
}

wint_t fputwc(wchar_t wc, FILE *f)
{
	REAL(fputwc);
	wide_used(f);
	return real_fputwc(wc, f);
}

wint_t putwc(wchar_t wc, FILE *f)
{
	REAL(putwc);
	wide_used(f);
	return real_putwc(wc, f);
}

int fputws(const wchar_t *restrict ws, FILE *restrict f)
{
	REAL(fputws);
	wide_used(f);
	return real_fputws(ws, f);
}

int vfwprintf(FILE *restrict f, const wchar_t *restrict fmt, va_list ap)
{
	REAL(vfwprintf);
	wide_used(f);
	return real_vfwprintf(f, fmt, ap);
}

int fwprintf(FILE *restrict f, const wchar_t *restrict fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vfwprintf(f, fmt, ap);
	va_end(ap);
	return r;
}
