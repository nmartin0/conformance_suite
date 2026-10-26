/*
 * open_memstream.c - planted faults in open_memstream() and
 * open_wmemstream(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "<function>.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 *
 * While a byte-stream fault is active, open_memstream() returns a stream of
 * this library's own, built on fopencookie() (a CI-only facility of the
 * reference C libraries), which follows the open_memstream() page except
 * for the one fault. A fault name not listed below gives the faithful
 * stream, so the stream itself can be checked: every test must PASS under
 * it. The wide-stream faults change what the real open_wmemstream()
 * reports after fflush() and fclose().
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

struct ms {
	const char *m;		/* the fault, fixed when the stream is opened */
	char **bufp;
	size_t *sizep;
	char *buf;
	size_t cap, pos, len;
	int wrote, closing, lose;
};

#define F(name) (c->m != NULL && strcmp(c->m, name) == 0)

/* Streams made or watched here. */
static struct {
	FILE *f;
	struct ms *c;		/* byte streams of this library */
	wchar_t **wbufp;	/* real wide streams */
	size_t *wsizep;
} seen[64];

static void remember(FILE *f, struct ms *c, wchar_t **wb, size_t *ws)
{
	int i;

	for (i = 0; i < 64; i++)
		if (seen[i].f == NULL) {
			seen[i].f = f, seen[i].c = c, seen[i].wbufp = wb, seen[i].wsizep = ws;
			return;
		}
}

static int slot(FILE *f)
{
	int i;

	for (i = 0; f != NULL && i < 64; i++)
		if (seen[i].f == f)
			return i;
	return -1;
}

static void publish(struct ms *c)
{
	*c->bufp = c->buf;
	*c->sizep = F("size_is_len") || c->pos > c->len ? c->len : c->pos;
	if (F("size_counts_nul"))
		*c->sizep += 1;		/* counts the terminating null */
}

/* Room for byte need - 1 and a null byte after it; new bytes are null. */
static int grow(struct ms *c, size_t need)
{
	size_t cap = c->cap;
	char *nb;

	if (need + 1 <= cap)
		return 0;
	while (cap < need + 1)
		cap = cap * 2 + 16;
	if (F("fixed_cap") && cap > 4096) {
		errno = ENOMEM;		/* never grows past 4 KiB */
		return -1;
	}
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

	if (c->lose)
		return (ssize_t)n;	/* drops what an unbuffered stream writes */
	if (grow(c, c->pos + n) != 0)
		return -1;
	if (c->pos > c->len && F("gap_garbage"))
		memset(c->buf + c->len, 'x', c->pos - c->len);	/* leaves the gap unset */
	memcpy(c->buf + c->pos, in, n);
	if (!F("pos_stuck"))
		c->pos += n;
	if (c->pos > c->len) {
		c->len = c->pos;
		c->buf[c->len] = F("no_nul") ? 'x' : 0;
	}
	c->wrote = 1;
	if (!(F("close_no_update") && c->closing))
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
		base = F("seek_end_zero") ? 0 : F("seek_end_cap") ? (long long)c->cap : (long long)c->len;
	else {
		errno = EINVAL;
		return -1;
	}
	np = base + (long long)*off;
	if (np < 0 || (F("seek_past_fails") && np > (long long)c->len)) {
		errno = EINVAL;
		return -1;
	}
	if (F("extends_garbage") && np > (long long)c->len) {
		if (grow(c, (size_t)np) != 0)
			return -1;
		memset(c->buf + c->len, 'x', (size_t)np - c->len);	/* extends without clearing */
		c->len = (size_t)np;
		c->buf[c->len] = 0;
	}
	if (F("seek_truncates") && np < (long long)c->len) {
		c->len = (size_t)np;	/* forgets the bytes after the new position */
		memset(c->buf + c->len, 0, c->cap - c->len);
	}
	c->pos = (size_t)np;
	if (!F("size_stale"))
		publish(c);
	*off = (off_t)np;
	return 0;
}

static int mclose(void *ck)
{
	struct ms *c = ck;
	int i;

	if (!F("size_stale") && !F("close_no_update") && !(F("size_unset") && !c->wrote))
		publish(c);
	for (i = 0; i < 64; i++)
		if (seen[i].c == c)
			seen[i].f = NULL, seen[i].c = NULL;
	free(c);
	return 0;
}

static FILE *ms_stream(char **bufp, size_t *sizep, const char *m)
{
	cookie_io_functions_t io = { NULL, mwrite, mseek, mclose };
	struct ms *c;
	FILE *f;

	c = calloc(1, sizeof *c);
	if (c == NULL)
		return NULL;
	c->m = m;
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
	if (!F("size_unset"))
		publish(c);
	if (F("wide_oriented"))
		(void)fwide(f, 1);	/* orients the byte stream for wide characters */
	else
		(void)fwide(f, -1);	/* byte-oriented, as the page says */
	remember(f, c, NULL, NULL);
	return f;
}

/* Faults that change the real open_wmemstream(). */
static int wide_fault(const char *m)
{
	return IS("wsize_bytes") || IS("wno_nul") || IS("wide_byte");
}

FILE *open_memstream(char **bufp, size_t *sizep)
{
	REAL(open_memstream);
	const char *m = mutant_of("open_memstream.");
	FILE *f;

	if (m == NULL || wide_fault(m))
		return real_open_memstream(bufp, sizep);
	if (IS("no_limit")) {
		/* Ignores {STREAM_MAX}: where the real call says EMFILE, opens anyway. */
		f = real_open_memstream(bufp, sizep);
		if (f != NULL || errno != EMFILE)
			return f;
	}
	return ms_stream(bufp, sizep, m);
}

FILE *open_wmemstream(wchar_t **bufp, size_t *sizep)
{
	REAL(open_wmemstream);
	const char *m = mutant_of("open_memstream.");
	FILE *f = real_open_wmemstream(bufp, sizep);

	if (f != NULL && m != NULL && wide_fault(m))
		remember(f, NULL, bufp, sizep);
	return f;
}

/* After a real flush or close of a watched wide stream. */
static void wide_after(const char *m, wchar_t **bufp, size_t *sizep)
{
	if (IS("wsize_bytes"))
		*sizep *= sizeof(wchar_t);	/* counts bytes, not wide characters */
	if (IS("wno_nul") && *bufp != NULL)
		(*bufp)[*sizep] = L'x';	/* no null wide character */
}

int fflush(FILE *f)
{
	REAL(fflush);
	const char *m = mutant_of("open_memstream.");
	int i = slot(f), r = real_fflush(f);

	if (r == 0 && i >= 0 && seen[i].wbufp != NULL)
		wide_after(m, seen[i].wbufp, seen[i].wsizep);
	return r;
}

int fclose(FILE *f)
{
	REAL(fclose);
	const char *m = mutant_of("open_memstream.");
	int i = slot(f), r;
	wchar_t **wb = NULL;
	size_t *ws = NULL;

	if (i >= 0) {
		wb = seen[i].wbufp, ws = seen[i].wsizep;
		if (seen[i].c != NULL)
			seen[i].c->closing = 1;
		if (wb != NULL)
			seen[i].f = NULL;
	}
	r = real_fclose(f);
	if (r == 0 && wb != NULL)
		wide_after(m, wb, ws);
	return r;
}

/* "wide_byte": wide memory streams report byte orientation. */
int fwide(FILE *f, int mode)
{
	REAL(fwide);
	const char *m = mutant_of("open_memstream.");
	int i = slot(f);

	if (IS("wide_byte") && mode == 0 && i >= 0 && seen[i].wbufp != NULL)
		return -1;
	/* "wide_oriented": where the stream cannot be reoriented after opening, report it. */
	if (IS("wide_oriented") && mode == 0 && i >= 0 && seen[i].c != NULL)
		return 1;
	return real_fwide(f, mode);
}

/* "setvbuf_loses": a memory stream made unbuffered loses its output. */
int setvbuf(FILE *restrict f, char *restrict buf, int type, size_t size)
{
	REAL(setvbuf);
	const char *m = mutant_of("open_memstream.");
	int i = slot(f);

	if (IS("setvbuf_loses") && type == _IONBF && i >= 0 && seen[i].c != NULL)
		seen[i].c->lose = 1;
	return real_setvbuf(f, buf, type, size);
}
