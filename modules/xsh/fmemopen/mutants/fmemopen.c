/*
 * fmemopen.c - planted faults in fmemopen(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "<function>.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 *
 * While a fault is active, fmemopen() returns a memory stream of this
 * library's own, built on fopencookie() (a CI-only facility of the
 * reference C libraries). Without the fault the stream follows the
 * fmemopen() page; each fault breaks one rule the way an implementation
 * might. Any fault name not listed below gives the faithful stream, which
 * is how the stream itself is checked: every test except XSH.fmemopen.024
 * (whose stream limit is the C library's, which fopencookie() may not
 * share) must PASS under it. Orientation is tracked here (see fwide()).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <wchar.h>

static char phase[16];

/* pc.c sets PC_PHASE with setenv(); remember it in this process. */
int setenv(const char *name, const char *value, int overwrite)
{
	static int (*real)(const char *, const char *, int);

	if (real == NULL)
		real = (int (*)(const char *, const char *, int))dlsym(RTLD_NEXT, "setenv");
	if (strcmp(name, "PC_PHASE") == 0 && strlen(value) < sizeof phase)
		strcpy(phase, value);
	return real(name, value, overwrite);
}

static const char *mutant_of(const char *prefix)
{
	const char *m = getenv("PC_MUTANT");
	size_t n = strlen(prefix);

	if (strcmp(phase, "test") != 0 || m == NULL || strncmp(m, prefix, n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

struct mem {
	const char *m;		/* the fault, fixed when the stream is opened */
	unsigned char *buf;
	size_t size, pos, end;
	char mode;		/* 'r', 'w' or 'a' */
	int plus, own;
	int used;		/* read or written through, or oriented by fwide() */
};

#define F(name) (c->m != NULL && strcmp(c->m, name) == 0)

/* The streams made here and their state, for the fwide() fault. */
static struct {
	FILE *f;
	struct mem *c;
} mine[64];

static ssize_t mread(void *ck, char *out, size_t n)
{
	struct mem *c = ck;
	size_t lim = F("read_to_max") ? c->size : c->end, k;
	unsigned char *z;

	c->used = 1;
	if (F("eof_early") && lim > 0)
		lim--;			/* off by one at the end */
	if (F("read_skip") && c->pos == 0 && lim > 0)
		c->pos = 1;		/* loses the first byte */
	if (c->pos >= lim)
		return 0;
	k = lim - c->pos < n ? lim - c->pos : n;
	if (F("read_stops_nul") && (z = memchr(c->buf + c->pos, 0, k)) != NULL)
		k = (size_t)(z - (c->buf + c->pos));	/* treats a null byte as the end */
	memcpy(out, c->buf + c->pos, k);
	c->pos += k;
	return (ssize_t)k;
}

static ssize_t mwrite(void *ck, const char *in, size_t n)
{
	struct mem *c = ck;
	size_t cap = F("overrun") ? c->size + 4 : c->size, k, want;
	int advanced;

	c->used = 1;
	if (n == 0)
		return 0;
	if (c->mode == 'a' && !F("a_write_at_pos"))
		c->pos = c->end;
	if (c->mode != 'a' && F("write_at_end"))
		c->pos = c->end;
	if (F("write_at_start"))
		c->pos = 0;
	if (c->pos >= cap) {
		errno = ENOSPC;
		return -1;
	}
	want = c->pos + n;
	k = cap - c->pos < n ? cap - c->pos : n;
	memcpy(c->buf + c->pos, in, k);
	c->pos += k;
	advanced = c->pos > c->end;
	if (F("end_unbounded") && want > c->end)
		c->end = want;		/* counts what did not fit */
	else if (F("r_end_follows") && c->mode == 'r')
		c->end = c->pos;	/* treats "r+" like "w+" */
	else if (F("end_shrinks"))
		c->end = c->pos;
	else if (advanced && !F("end_fixed"))
		c->end = c->pos;
	if (advanced && !F("no_nul_after") && c->end < c->size)
		c->buf[c->end] = 0;
	if (F("nul_overrun") && c->pos == c->size)
		c->buf[c->size] = 0;	/* terminates even when it does not fit */
	if (F("nul_in_last") && c->pos == c->size && !c->plus && c->size > 0)
		c->buf[c->size - 1] = 0;	/* terminates over the last byte */
	if (F("nul_always") && c->pos < c->size)
		c->buf[c->pos] = 0;	/* after every write */
	return (ssize_t)k;
}

static int mseek(void *ck, off_t *off, int whence)
{
	struct mem *c = ck;
	long long base, np, lim;

	if (whence == SEEK_SET)
		base = 0;
	else if (whence == SEEK_CUR)
		base = (long long)c->pos;
	else if (whence == SEEK_END)
		base = (long long)(F("seek_end_max") ? c->size : c->end);
	else {
		errno = EINVAL;
		return -1;
	}
	np = base + (long long)*off;
	lim = (long long)(F("end_unbounded") && c->end > c->size ? c->end : c->size);
	if (np < 0 || (np > lim && !F("seek_unbounded")) || (np > (long long)c->end && F("seek_past_end_fails"))) {
		errno = EINVAL;
		return -1;
	}
	c->pos = (size_t)np;
	*off = (off_t)np;
	return 0;
}

static int mclose(void *ck)
{
	struct mem *c = ck;
	int i;

	if (F("r_terminates") && c->mode == 'r' && c->size > 0)
		c->buf[c->pos < c->size ? c->pos : c->size - 1] = 0;	/* terminates the string whatever the mode */
	if (c->own)
		free(c->buf);
	for (i = 0; i < 64; i++)
		if (mine[i].c == c)
			mine[i].f = NULL, mine[i].c = NULL;
	free(c);
	return 0;
}

static FILE *mem_stream(void *buf, size_t size, const char *mode, const char *m)
{
	cookie_io_functions_t io = { mread, mwrite, mseek, mclose };
	struct mem *c;
	const char *how;
	FILE *f;
	int i;

	if (mode[0] != 'r' && mode[0] != 'w' && mode[0] != 'a') {
		errno = EINVAL;
		return NULL;
	}
	c = calloc(1, sizeof *c);
	if (c == NULL)
		return NULL;
	c->m = m;
	c->mode = mode[0];
	c->plus = strchr(mode, '+') != NULL;
	c->size = size;
	if (buf == NULL) {
		if (F("null_short"))
			c->size = size / 2;	/* allocates too little */
		c->buf = malloc(c->size + 1);
		if (c->buf == NULL) {
			free(c);
			errno = ENOMEM;
			return NULL;
		}
		memset(c->buf, F("null_append_garbage") ? 'x' : 0, c->size + 1);
		c->own = 1;
	} else
		c->buf = buf;
	switch (c->mode) {
	case 'r':
		c->end = c->size;
		if (F("r_end_strlen"))
			c->end = strnlen((char *)c->buf, c->size);
		if (F("rw_pos_end"))
			c->pos = c->end;
		break;
	case 'w':
		if (c->size > 0 && !(F("w_no_nul") && !c->plus) && !(F("wplus_no_nul") && c->plus))
			c->buf[0] = 0;
		if (F("zero_nul") && c->size == 0)
			c->buf[0] = 0;	/* truncates even a buffer of no bytes */
		if (F("w_end_max"))
			c->end = c->size;
		break;
	default:
		if (c->own && !F("null_append_garbage"))
			c->end = 0;
		else {
			c->end = strnlen((char *)c->buf, c->size);
			if (c->end == c->size && F("a_nonul_zero"))
				c->end = 0;
			if (c->end == c->size && F("a_nonul_last") && c->size > 0)
				c->end = c->size - 1;
		}
		c->pos = c->end;
		if (F("a_pos_start"))
			c->pos = 0;
		if (F("a_end_max"))
			c->end = c->size;
		break;
	}
	if (c->own && F("null_pos_end"))
		c->pos = c->size;	/* starts an allocated buffer at its end */
	if (c->mode == 'r')
		how = c->plus || F("r_accepts_writes") ? "r+" : "r";
	else
		how = c->plus ? "w+" : "w";
	f = fopencookie(c, how, io);
	if (f == NULL) {
		if (c->own)
			free(c->buf);
		free(c);
		return NULL;
	}
	if (F("byte_oriented"))
		(void)fwide(f, -1);
	for (i = 0; i < 64; i++)
		if (mine[i].f == NULL) {
			mine[i].f = f;
			mine[i].c = c;
			break;
		}
	return f;
}

FILE *fmemopen(void *restrict buf, size_t size, const char *restrict mode)
{
	REAL(fmemopen);
	const char *m = mutant_of("fmemopen.");
	FILE *f;

	if (m == NULL)
		return real_fmemopen(buf, size, mode);
	if (IS("zero_wrong_errno") && size == 0) {
		errno = ENOMEM;		/* refuses size zero with the wrong error */
		return NULL;
	}
	if (IS("no_limit")) {
		/* Ignores {STREAM_MAX}: where the real call says EMFILE, opens anyway. */
		f = real_fmemopen(buf, size, mode);
		if (f != NULL || errno != EMFILE)
			return f;
	}
	return mem_stream(buf, size, mode, m);
}

/*
 * Orientation of this library's streams. A stream has none until its first
 * input or output (XSH 2.5: as for file streams), but some fopencookie()
 * streams are byte-oriented from creation, so until the cookie has been
 * read or written (or fwide() has oriented it) a query answers 0.
 * "byte_oriented": the stream is byte-oriented at open.
 * "wide_after_read": memory streams never take an orientation.
 */
int fwide(FILE *f, int mode)
{
	REAL(fwide);
	const char *m = mutant_of("fmemopen.");
	int i;

	for (i = 0; m != NULL && i < 64; i++)
		if (mine[i].f != NULL && mine[i].f == f) {
			if (mode != 0)
				mine[i].c->used = 1;
			else if (IS("wide_after_read") || (!mine[i].c->used && !IS("byte_oriented")))
				return 0;
			break;
		}
	return real_fwide(f, mode);
}
