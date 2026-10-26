/*
 * readdir.c - conforming variants of the directory stream functions
 * (opendir(), fdopendir(), readdir(), readdir_r(), rewinddir(), telldir(),
 * seekdir(), dirfd(), closedir(), scandir()), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Every variant replaces the
 * system's directory streams with an emulated one built on the real
 * functions, so that the choices the standard leaves open can be made
 * differently from the real library, consistently across all the functions
 * that see a stream:
 *
 * "snapshot": the stream reads the whole directory at once, when it is
 *   opened (opendir(), fdopendir()) and at each rewinddir(), and readdir()
 *   then hands out the buffered entries. XSH readdir() allows any number
 *   of entries per actual read, and leaves it unspecified whether a file
 *   added or removed after the last opendir() or rewinddir() is seen; the
 *   access time is marked when the directory is actually read, which is
 *   at the open. rewinddir() must show the current state (XSH
 *   rewinddir()), so it reads the directory again. Every other variant
 *   below is built on this one.
 * "reversed": the entries come in the reverse of the system's order. XSH
 *   readdir() says a stream is an ordered sequence of the entries but not
 *   which order.
 * "no_dots": no entries for dot and dot-dot. XSH readdir() returns them
 *   only if they exist in the directory, which the file system decides;
 *   pathname resolution of dot and dot-dot (XBD 4.16) does not depend on
 *   them.
 * "lazy_fd": opendir() keeps no file descriptor; dirfd() opens one when
 *   first asked, as if with O_RDONLY | O_DIRECTORY | O_CLOEXEC, and
 *   closedir() closes it (XSH opendir() "if opendir() opens a file
 *   descriptor", XSH dirfd() DESCRIPTION, XSH closedir()). In this model
 *   fdopendir() also sets FD_CLOEXEC on the descriptor it is given, which
 *   XSH fdopendir() leaves unspecified.
 * "odd_cookies": telldir() locations are negative and not consecutive.
 *   XSH telldir() says only that the value is the stream's location
 *   (a long, with no value reserved for errors), and seekdir() needs only
 *   values that telldir() returned.
 *
 * "sel_twice": scandir() calls sel twice for each entry (as one that
 *   reads the directory twice, to count and then to fill, would), keeping
 *   the second answer; the result is the same. XSH scandir() says sel is
 *   called on each entry but not that it is called only once (see the
 *   disputed XSH.alphasort.010).
 *
 * scandir() is emulated on the emulated streams (the C libraries call
 * their own internal functions), and stores trimmed records holding just
 * the name: XSH scandir() allocates them as if by malloc() and says
 * nothing of their size.
 */
#define _GNU_SOURCE 1
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pcvariant.h"

struct vent {
	struct dirent *d;	/* a trimmed copy: header and name */
};

struct vdir {
	struct vdir *next;
	DIR *real;		/* the system's stream, or NULL (lazy_fd, closed) */
	int fd;			/* a descriptor dirfd() opened (lazy_fd), or -1 */
	char *path;		/* absolute name of the directory (lazy_fd) */
	struct vent *e;
	size_t n, pos;
	int err;		/* an error met while reading, reported at the end */
	int seeked;		/* the last operation was seekdir(seekloc) */
	long seekloc;
	union {
		struct dirent d;
		char pad[sizeof(struct dirent) + 4096];
	} out;
};

static struct vdir *all;

static const char *var(void)
{
	const char *v = variant_of("readdir.");

	if (IS("snapshot") || IS("reversed") || IS("no_dots") || IS("lazy_fd") || IS("odd_cookies") ||
	    IS("sel_twice"))
		return v;
	return NULL;
}

static struct vdir *ours(DIR *d)
{
	struct vdir *p;

	for (p = all; p != NULL; p = p->next)
		if ((DIR *)(void *)p == d)
			return p;
	return NULL;
}

static void drop_entries(struct vdir *p)
{
	size_t i;

	for (i = 0; i < p->n; i++)
		free(p->e[i].d);
	free(p->e);
	p->e = NULL;
	p->n = p->pos = 0;
	p->err = 0;
}

static struct dirent *trimmed(const struct dirent *e)
{
	size_t len = strlen(e->d_name), size = offsetof(struct dirent, d_name) + len + 1;
	struct dirent *c = malloc(size);

	if (c != NULL) {
		memcpy(c, e, offsetof(struct dirent, d_name));
		memcpy(c->d_name, e->d_name, len + 1);
	}
	return c;
}

/* Read the rest of real into p (an actual read of the whole directory). */
static void load(struct vdir *p, DIR *real)
{
	const char *v = var();
	struct dirent *e;
	size_t cap = 0, i;
	REAL(readdir);

	drop_entries(p);
	for (;;) {
		errno = 0;
		e = real_readdir(real);
		if (e == NULL) {
			p->err = errno;
			break;
		}
		if (IS("no_dots") && (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0))
			continue;
		if (p->n == cap) {
			struct vent *ne = realloc(p->e, (cap = cap ? 2 * cap : 64) * sizeof *ne);
			if (ne == NULL) {
				p->err = ENOMEM;
				break;
			}
			p->e = ne;
		}
		if ((p->e[p->n].d = trimmed(e)) == NULL) {
			p->err = ENOMEM;
			break;
		}
		p->n++;
	}
	if (IS("reversed"))
		for (i = 0; i < p->n / 2; i++) {
			struct vent t = p->e[i];
			p->e[i] = p->e[p->n - 1 - i];
			p->e[p->n - 1 - i] = t;
		}
}

static DIR *adopt(DIR *real, const char *name)
{
	const char *v = var();
	struct vdir *p = calloc(1, sizeof *p);
	REAL(closedir);

	if (p == NULL) {
		(void)real_closedir(real);
		errno = ENOMEM;
		return NULL;
	}
	p->real = real;
	p->fd = -1;
	load(p, real);
	if (IS("lazy_fd") && name != NULL && (p->path = realpath(name, NULL)) != NULL) {
		(void)real_closedir(real);	/* no descriptor stays with the stream */
		p->real = NULL;
	}
	p->next = all;
	all = p;
	return (DIR *)(void *)p;
}

DIR *opendir(const char *name)
{
	int e = errno;
	DIR *real;
	DIR *r;
	REAL(opendir);

	real = real_opendir(name);
	if (real == NULL || var() == NULL)
		return real;
	r = adopt(real, name);
	if (r != NULL)
		errno = e;
	return r;
}

DIR *fdopendir(int fd)
{
	const char *v = var();
	int e = errno;
	DIR *real, *r;
	REAL(fdopendir);

	real = real_fdopendir(fd);
	if (real == NULL || v == NULL)
		return real;
	if (IS("lazy_fd"))
		(void)fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) | FD_CLOEXEC);
	r = adopt(real, NULL);
	if (r != NULL)
		errno = e;
	return r;
}

struct dirent *readdir(DIR *d)
{
	struct vdir *p = ours(d);
	const struct dirent *e;
	REAL(readdir);

	if (p == NULL)
		return real_readdir(d);
	p->seeked = 0;
	if (p->pos >= p->n) {
		if (p->err != 0)
			errno = p->err;
		return NULL;
	}
	e = p->e[p->pos++].d;
	memcpy(&p->out.d, e, offsetof(struct dirent, d_name));
	strcpy(p->out.d.d_name, e->d_name);
	return &p->out.d;
}

int readdir_r(DIR *d, struct dirent *entry, struct dirent **result)
{
	struct vdir *p = ours(d);
	const struct dirent *e;
	REAL(readdir_r);

	if (p == NULL)
		return real_readdir_r(d, entry, result);
	p->seeked = 0;
	if (p->pos >= p->n) {
		*result = NULL;
		return p->err;
	}
	e = p->e[p->pos++].d;
	memcpy(entry, e, offsetof(struct dirent, d_name) + strlen(e->d_name) + 1);
	*result = entry;
	return 0;
}

void rewinddir(DIR *d)
{
	struct vdir *p = ours(d);
	int e = errno;
	REAL(rewinddir);
	REAL(opendir);
	REAL(closedir);

	if (p == NULL) {
		real_rewinddir(d);
		return;
	}
	p->seeked = 0;
	if (p->real != NULL) {
		real_rewinddir(p->real);
		load(p, p->real);
	} else {
		DIR *t = real_opendir(p->path);

		if (t != NULL) {
			load(p, t);
			(void)real_closedir(t);
		} else {
			drop_entries(p);
		}
	}
	errno = e;
}

/* The location of position pos, and back. */
static long cookie(size_t pos)
{
	const char *v = var();

	if (IS("odd_cookies"))
		return -1000L - 13L * (long)pos;
	return (long)pos;
}

static size_t position(const struct vdir *p, long loc)
{
	const char *v = var();

	if (IS("odd_cookies")) {
		if (loc > -1000L || (-1000L - loc) % 13 != 0)
			return p->n;	/* not from telldir(): unspecified, here the end */
		loc = (-1000L - loc) / 13;
	}
	if (loc < 0 || (size_t)loc > p->n)
		return p->n;
	return (size_t)loc;
}

long telldir(DIR *d)
{
	struct vdir *p = ours(d);
	REAL(telldir);

	if (p == NULL)
		return real_telldir(d);
	if (p->seeked)
		return p->seekloc;	/* XSH telldir(): the value seekdir() was given */
	return cookie(p->pos);
}

void seekdir(DIR *d, long loc)
{
	struct vdir *p = ours(d);
	REAL(seekdir);

	if (p == NULL) {
		real_seekdir(d, loc);
		return;
	}
	p->pos = position(p, loc);
	p->seeked = 1;
	p->seekloc = loc;
}

int dirfd(DIR *d)
{
	struct vdir *p = ours(d);
	REAL(dirfd);

	if (p == NULL)
		return real_dirfd(d);
	if (p->real != NULL)
		return real_dirfd(p->real);
	if (p->fd < 0)
		p->fd = open(p->path, O_RDONLY | O_DIRECTORY | O_CLOEXEC);
	return p->fd;
}

int closedir(DIR *d)
{
	struct vdir *p = ours(d), **pp;
	int r = 0;
	REAL(closedir);

	if (p == NULL)
		return real_closedir(d);
	for (pp = &all; *pp != p; pp = &(*pp)->next)
		;
	*pp = p->next;
	if (p->real != NULL)
		r = real_closedir(p->real);
	if (p->fd >= 0 && close(p->fd) != 0)
		r = -1;
	drop_entries(p);
	free(p->path);
	free(p);
	return r;
}

int scandir(const char *dir, struct dirent ***namelist, int (*sel)(const struct dirent *),
	    int (*compar)(const struct dirent **, const struct dirent **))
{
	struct dirent **list = NULL, **nl, *e;
	size_t n = 0, cap = 0, i;
	int err = errno;
	const char *v = var();
	DIR *d;
	REAL(scandir);

	if (v == NULL)
		return real_scandir(dir, namelist, sel, compar);
	if ((d = opendir(dir)) == NULL)
		return -1;
	for (;;) {
		errno = 0;
		if ((e = readdir(d)) == NULL) {
			if (errno != 0)
				goto fail;
			break;
		}
		if (sel != NULL && IS("sel_twice"))
			(void)sel(e);	/* a first pass's call; the second decides */
		if (sel != NULL && sel(e) == 0)
			continue;
		if (n == cap) {
			nl = realloc(list, (cap = cap ? 2 * cap : 16) * sizeof *nl);
			if (nl == NULL)
				goto nomem;
			list = nl;
		}
		if ((list[n] = trimmed(e)) == NULL)
			goto nomem;
		n++;
	}
	(void)closedir(d);
	if (n > 0 && compar != NULL)
		qsort(list, n, sizeof *list, (int (*)(const void *, const void *))compar);
	if (list == NULL && (list = malloc(sizeof *list)) == NULL) {
		errno = ENOMEM;
		return -1;
	}
	*namelist = list;
	errno = err;
	return (int)n;
nomem:
	errno = ENOMEM;
fail:
	err = errno;
	(void)closedir(d);
	for (i = 0; i < n; i++)
		free(list[i]);
	free(list);
	errno = err;
	return -1;
}
