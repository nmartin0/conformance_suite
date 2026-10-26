/*
 * fopen.c - conforming variants of the stream functions (fopen(), fdopen(),
 * fflush(), fclose()) and of the file system under them, for the variant
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant models a whole
 * conforming system and says why the standard allows it. The variants are
 * used by the tests of xsh/fopen, xsh/fdopen, xsh/fflush and xsh/fclose.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "tiny_buffer": the buffer a stream on a regular file gets automatically
 * holds only 2 bytes. Nothing in XSH fopen(), XSH 2.5 or <stdio.h> sets the
 * size of an automatically allocated buffer (BUFSIZ is only the size
 * setbuf() uses), so a fully buffered stream may send its data in blocks
 * of 2 bytes. Streams on other files (terminals) keep the C library's own
 * choice, which the interactive-device rule governs.
 *
 * "read_all": the automatic buffer of a stream on a regular file opened
 * for reading is 2 MiB, so one read takes in the whole of the tests' 1 MiB
 * files and leaves the descriptor at the end of the file with the stream's
 * end-of-file indicator still clear. The buffer size is the
 * implementation's choice (as above). This system reads "the file is not
 * already at EOF" in XSH fflush() and fclose() as the file offset being at
 * the end of the file (one of the two readings of Q-29): there fflush() of
 * an input stream does nothing, and fclose() leaves the offset where the
 * reads put it. Under the other reading this would not conform, so a test
 * meeting it must not FAIL: it can only be UNRESOLVED (RULES.md 8).
 */
#define TINY 2		/* bytes the stream itself can hold */
#define ALL (2L << 20)

static int regular(int fd, struct stat *st)
{
	return fd >= 0 && fstat(fd, st) == 0 && S_ISREG(st->st_mode);
}

static FILE *rebuffer(FILE *f, const char *mode)
{
	const char *v = variant_of("fopen.");
	struct stat st;
	char *b;

	if (f == NULL || !regular(fileno(f), &st))
		return f;
	if (IS("tiny_buffer")) {
#ifdef __GLIBC__
		size_t n = TINY;
#else
		size_t n = TINY + 8;	/* some libraries keep 8 bytes of a caller's buffer for pushback */
#endif
		if ((b = malloc(n)) != NULL)
			(void)setvbuf(f, b, _IOFBF, n);
	}
	if (IS("read_all") && (mode[0] == 'r' || strchr(mode, '+') != NULL)) {
		if ((b = malloc((size_t)ALL)) != NULL)
			(void)setvbuf(f, b, _IOFBF, (size_t)ALL);
	}
	return f;
}

/* read_all: an input stream whose descriptor is at the end of the file. */
static int input_at_end(FILE *f)
{
	struct stat st;
	off_t at;

	if (f == NULL || !__freading(f) || !regular(fileno(f), &st))
		return 0;
	at = lseek(fileno(f), 0, SEEK_CUR);
	return at != -1 && at == st.st_size;
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
	const char *v = variant_of("fopen.");
	int p, r, i;
	REAL(ungetc);

	if (!IS("read_all"))
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

/*
 * "eilseq": a file whose last pathname component is not a portable
 * filename cannot be created here (as on a file system that stores names
 * in a fixed encoding). XSH fopen() ERRORS (EILSEQ) then requires fopen()
 * with 'w' or 'a' of such a new name to fail with EILSEQ; open() with
 * O_CREAT reports the same (XSH open() ERRORS, EILSEQ). Existing files
 * are not affected.
 */
static int portable_last(const char *path)
{
	const char *p = strrchr(path, '/');
	const unsigned char *s = (const unsigned char *)(p != NULL ? p + 1 : path);

	for (; *s != '\0'; s++)
		if (!((*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z') || (*s >= '0' && *s <= '9') ||
		      *s == '.' || *s == '_' || *s == '-'))
			return 0;
	return 1;
}

static int cannot_create(const char *path)
{
	struct stat st;

	return !portable_last(path) && lstat(path, &st) == -1 && errno == ENOENT;
}

/*
 * "x_exclusive": 'x' gives exclusive access in an implementation-defined
 * form, by implementation-specific open() flags (XSH fopen(), the 'x'
 * character and the paragraph after the flags table). Here such a stream's
 * descriptor carries a private status flag that fcntl(F_GETFL) reports,
 * and an advisory lock on the whole file. Permissions are as without 'x'.
 */
#define PRIVATE_FLAG 0x40000000
static int xfds[64];
static int nx;

static FILE *exclusive(FILE *f, const char *mode)
{
	const char *v = variant_of("fopen.");

	if (IS("x_exclusive") && f != NULL && (mode[0] == 'w' || mode[0] == 'a') && strchr(mode, 'x') != NULL) {
		(void)lockf(fileno(f), F_TLOCK, 0);
		if (nx < 64)
			xfds[nx++] = fileno(f);
	}
	return f;
}

static void forget(int fd)
{
	int i;

	for (i = 0; i < nx; i++)
		if (xfds[i] == fd)
			xfds[i] = xfds[--nx];
}

FILE *fopen(const char *restrict path, const char *restrict mode)
{
	const char *v = variant_of("fopen.");
	REAL(fopen);

	if (IS("eilseq") && (mode[0] == 'w' || mode[0] == 'a') && cannot_create(path)) {
		errno = EILSEQ;
		return NULL;
	}
	return exclusive(rebuffer(real_fopen(path, mode), mode), mode);
}

FILE *fdopen(int fd, const char *mode)
{
	REAL(fdopen);

	return rebuffer(real_fdopen(fd, mode), mode);
}

int open(const char *path, int flags, ...)
{
	const char *v = variant_of("fopen.");
	mode_t m = 0;
	va_list ap;
	REAL(open);

	if (flags & O_CREAT) {
		va_start(ap, flags);
		m = (mode_t)va_arg(ap, int);
		va_end(ap);
		if (IS("eilseq") && cannot_create(path)) {
			errno = EILSEQ;
			return -1;
		}
	}
	return real_open(path, flags, m);
}

int fcntl(int fd, int cmd, ...)
{
	const char *v = variant_of("fopen.");
	va_list ap;
	long arg;
	int r, i;
	REAL(fcntl);

	va_start(ap, cmd);
	arg = va_arg(ap, long);
	va_end(ap);
	r = real_fcntl(fd, cmd, arg);
	if (IS("x_exclusive") && cmd == F_GETFL && r != -1)
		for (i = 0; i < nx; i++)
			if (xfds[i] == fd)
				r |= PRIVATE_FLAG;
	return r;
}

int fflush(FILE *f)
{
	const char *v = variant_of("fopen.");
	REAL(fflush);

	if (IS("read_all") && f != NULL && input_at_end(f)) {
		pb_discard(f);
		return 0;
	}
	if (f != NULL)
		pb_forget(f);
	return real_fflush(f);
}

int fclose(FILE *f)
{
	const char *v = variant_of("fopen.");
	REAL(fclose);

	if (IS("x_exclusive"))
		forget(fileno(f));
	pb_forget(f);
	if (IS("read_all") && input_at_end(f)) {
		off_t at = lseek(fileno(f), 0, SEEK_CUR);
		int d = dup(fileno(f)), r, e;

		r = real_fclose(f);
		e = errno;
		if (d >= 0) {
			(void)lseek(d, at, SEEK_SET);	/* the offset stays where the reads left it */
			(void)close(d);
		}
		errno = e;
		return r;
	}
	return real_fclose(f);
}

/*
 * "coarse_times": the file system keeps timestamps to whole seconds. XBD
 * 4.12 makes the resolution implementation-defined, no coarser than one
 * second, and a value is truncated to it; so stat() and its relatives
 * report tv_nsec 0.
 */
static int coarse(int r, struct stat *st)
{
	const char *v = variant_of("fopen.");

	if (IS("coarse_times") && r == 0) {
		st->st_atim.tv_nsec = 0;
		st->st_mtim.tv_nsec = 0;
		st->st_ctim.tv_nsec = 0;
	}
	return r;
}

int stat(const char *restrict path, struct stat *restrict st)
{
	REAL(stat);
	return coarse(real_stat(path, st), st);
}

int lstat(const char *restrict path, struct stat *restrict st)
{
	REAL(lstat);
	return coarse(real_lstat(path, st), st);
}

int fstat(int fd, struct stat *st)
{
	REAL(fstat);
	return coarse(real_fstat(fd, st), st);
}

int fstatat(int dirfd, const char *restrict path, struct stat *restrict st, int flag)
{
	REAL(fstatat);
	return coarse(real_fstatat(dirfd, path, st, flag), st);
}

/* read_all: a seek discards pushback itself; forget what was tracked. */
int fseek(FILE *f, long off, int whence)
{
	REAL(fseek);
	pb_forget(f);
	return real_fseek(f, off, whence);
}

int fseeko(FILE *f, off_t off, int whence)
{
	REAL(fseeko);
	pb_forget(f);
	return real_fseeko(f, off, whence);
}

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
