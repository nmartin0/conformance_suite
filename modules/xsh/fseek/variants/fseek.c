/*
 * fseek.c - conforming variants of fseek(), fseeko() and of the stream
 * positioning they rely on, for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "pcvariant.h"

/* The stream's descriptor, or -1 (a memory stream), leaving errno alone. */
static int fd_of(FILE *f)
{
	int e = errno, fd = fileno(f);

	errno = e;
	return fd;
}

/*
 * "lseek_first": before it writes any pending output, fseek() asks the
 * descriptor for its offset; if that fails, fseek() fails with lseek()'s
 * error at once and writes nothing. XSH 2.3 leaves the order in which
 * errors are detected open and lets a failed call have done only part of
 * its work, so the pending data may stay unwritten. Then no write error
 * has happened, and fseek() sets the error indicator only for a read or
 * write error (XSH fseek(), DESCRIPTION), so the indicator stays clear.
 * On a pipe this gives ESPIPE before any EPIPE or EAGAIN could arise; on a
 * closed descriptor, EBADF.
 */
static int lseek_fails(FILE *f)
{
	int fd = fd_of(f);

	return fd >= 0 && lseek(fd, 0, SEEK_CUR) == -1;
}

/*
 * "mem_fail_clamps": a seek of an fmemopen() stream outside the buffer
 * fails with EINVAL, but leaves the position at the bound it ran into (0
 * for a target before the start, the end position otherwise). The
 * position after a failed fseek() is not stated (XSH fseek(), fmemopen()),
 * and XSH 2.3 lets a failed call have done part of its work; positions
 * stay within the buffer, as XSH 2.5 requires.
 */
static off_t (*real_ftello)(FILE *);

static int clamp(FILE *f, off_t off, int whence)
{
	REAL(fseeko);
	off_t at = real_ftello(f);
	int below;

	below = (whence == SEEK_SET && off < 0) || (whence == SEEK_CUR && at != -1 && at + off < 0) ||
		(whence == SEEK_END && off < 0);
	(void)real_fseeko(f, 0, below ? SEEK_SET : SEEK_END);
	errno = EINVAL;
	return -1;
}


/*
 * Pushed-back bytes not yet read. XSH ungetc() requires a successful
 * fflush() to discard pushback whether or not the stream is at end of
 * file, so the fflush() of this variant that otherwise does nothing still discards
 * them: it reads them off with the real fgetc(), which leaves the stream
 * where it was before the bytes were pushed (ungetc(): the position after
 * all pushed-back bytes are read). Seeks and fclose() forget them.
 */
static struct { FILE *f; off_t pos; int n; } pb[16];

static int pb_slot(FILE *f, int make)
{
	int i, free_slot = -1;

	for (i = 0; i < 16; i++) {
		if (pb[i].f == f)
			return i;
		if (pb[i].f == NULL && free_slot < 0)
			free_slot = i;
	}
	if (!make)
		return -1;
	i = free_slot >= 0 ? free_slot : 0;
	pb[i].f = f;
	return i;
}

static int pb_pending(FILE *f)
{
	int i = pb_slot(f, 0);
	off_t at, d;

	if (i < 0)
		return 0;
	at = ftello(f);
	d = at - pb[i].pos;
	return at == -1 || d < 0 || d > pb[i].n ? 0 : (int)(pb[i].n - d);
}

static void pb_forget(FILE *f)
{
	int i = pb_slot(f, 0);

	if (i >= 0)
		pb[i].f = NULL;
}

static void pb_discard(FILE *f)
{
	int k = pb_pending(f);

	pb_forget(f);
	while (k-- > 0)
		(void)fgetc(f);
}

int ungetc(int c, FILE *f)
{
	const char *v = variant_of("fseek.");
	int p, r, i;
	REAL(ungetc);

	if (!IS("fflush_q29"))
		return real_ungetc(c, f);
	p = pb_pending(f);
	r = real_ungetc(c, f);
	if (r != EOF) {
		i = pb_slot(f, 1);
		pb[i].pos = ftello(f);
		pb[i].n = p + 1;
	}
	return r;
}

int fseek(FILE *f, long off, int whence)
{
	const char *v = variant_of("fseek.");
	REAL(fseek);
	int r;

	pb_forget(f);

	if (real_ftello == NULL)
		real_ftello = (off_t (*)(FILE *))dlsym(RTLD_NEXT, "ftello");
	if (IS("lseek_first") && lseek_fails(f))
		return -1;
	r = real_fseek(f, off, whence);
	if (IS("mem_fail_clamps") && r != 0 && errno == EINVAL && fd_of(f) < 0)
		return clamp(f, off, whence);
	return r;
}

int fseeko(FILE *f, off_t off, int whence)
{
	const char *v = variant_of("fseek.");
	REAL(fseeko);
	int r;

	pb_forget(f);

	if (real_ftello == NULL)
		real_ftello = (off_t (*)(FILE *))dlsym(RTLD_NEXT, "ftello");
	if (IS("lseek_first") && lseek_fails(f))
		return -1;
	r = real_fseeko(f, off, whence);
	if (IS("mem_fail_clamps") && r != 0 && errno == EINVAL && fd_of(f) < 0)
		return clamp(f, off, whence);
	return r;
}

/*
 * "fflush_q29": fflush() of a stream open for reading does nothing when
 * the descriptor's offset is already at the end of the file. fflush() sets
 * the offset (and discards pushback) only "if the file is not already at
 * EOF"; whether that means the stream's end-of-file indicator or the file
 * offset is open (Q-29). This plants the offset reading: after read-ahead
 * has taken a whole small file, the offset is not moved back.
 */
int fflush(FILE *f)
{
	const char *v = variant_of("fseek.");
	REAL(fflush);
	struct stat st;
	int fd;

	if (IS("fflush_q29") && f != NULL && __freading(f) && (fd = fd_of(f)) >= 0 && fstat(fd, &st) == 0 &&
	    S_ISREG(st.st_mode) && lseek(fd, 0, SEEK_CUR) >= st.st_size) {
		pb_discard(f);
		return 0;
	}
	if (f != NULL)
		pb_forget(f);
	return real_fflush(f);
}

/* fflush_q29: these discard pushback themselves; forget what was tracked. */
int fsetpos(FILE *f, const fpos_t *pos)
{
	REAL(fsetpos);
	pb_forget(f);
	return real_fsetpos(f, pos);
}

void rewind(FILE *f)
{
	REAL(rewind);
	pb_forget(f);
	real_rewind(f);
}

int fclose(FILE *f)
{
	REAL(fclose);
	pb_forget(f);
	return real_fclose(f);
}
