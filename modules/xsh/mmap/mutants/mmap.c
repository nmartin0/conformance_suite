/*
 * mmap.c - planted faults in mmap(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "mmap.<fault>" and apply only while
 * PC_PHASE is "test". Some faults also wrap munmap() and close(), which
 * see the mappings this library recorded.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "mmap.", 5) != 0)
		return NULL;
	return m + 5;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

static void *fail(int e)
{
	errno = e;
	return MAP_FAILED;
}

/* File mappings made in the test phase: the descriptor given and a duplicate. */
static struct rec {
	char *addr;
	size_t len;
	int fd, dupfd, flags;
	off_t off;
} recs[64];
static int nrecs;

static long page(void)
{
	return sysconf(_SC_PAGESIZE);
}

void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off)
{
	REAL(mmap);
	REAL(mprotect);
	const char *m = mutant();
	int anon = (flags & MAP_ANONYMOUS) != 0;
	char *p;

	if (m == NULL)
		return real_mmap(addr, len, prot, flags, fd, off);
	if (IS("offset_ignored") && !anon)
		off = 0;	/* maps the start of the file */
	if (IS("shared_as_private") && (flags & MAP_SHARED))
		flags = (flags & ~MAP_SHARED) | MAP_PRIVATE;
	if (IS("private_as_shared") && (flags & MAP_PRIVATE))
		flags = (flags & ~MAP_PRIVATE) | MAP_SHARED;
	if (IS("private_needs_write") && (flags & MAP_PRIVATE) && (prot & PROT_WRITE) && !anon &&
	    (fcntl(fd, F_GETFL) & O_ACCMODE) == O_RDONLY)
		return fail(EACCES);	/* demands write access as for MAP_SHARED */
	if (IS("anon_needs_fd") && anon && fd == -1)
		return fail(EBADF);
	if (IS("len0_ok") && len == 0)
		len = 1;
	if (IS("default_shared") && !(flags & (MAP_SHARED | MAP_PRIVATE)))
		flags |= MAP_SHARED;
	if (IS("prot_ignored") && (anon || (fcntl(fd, F_GETFL) & O_ACCMODE) == O_RDWR))
		prot = PROT_READ | PROT_WRITE;	/* no protection hardware */
	if (IS("none_as_read") && prot == PROT_NONE)
		prot = PROT_READ;
	if (IS("write_only_enotsup") && prot == PROT_WRITE)
		return fail(ENOTSUP);
	if (IS("none_enotsup") && prot == PROT_NONE)
		return fail(ENOTSUP);
	if (IS("fixed_ignored"))
		flags &= ~MAP_FIXED;
	if (IS("hint_as_fixed") && addr != NULL)
		flags |= MAP_FIXED;
	p = real_mmap(addr, len, prot, flags, fd, off);
	if (p == MAP_FAILED) {
		if (IS("badfd_einval") && errno == EBADF)
			errno = EINVAL;
		if (IS("eacces_as_eperm") && errno == EACCES)
			errno = EPERM;
		if (IS("enodev_as_einval") && errno == ENODEV)
			errno = EINVAL;
		if (IS("enomem_as_einval") && errno == ENOMEM)
			errno = EINVAL;
		return p;
	}
	if (IS("anon_garbage") && anon) {
		if (real_mprotect(p, len, PROT_READ | PROT_WRITE) == 0)
			memset(p, 0xa5, len);	/* memory handed out uncleared */
		(void)real_mprotect(p, len, prot);
	}
	if (IS("partial_page_dropped") && len % (size_t)page() != 0 && len > (size_t)page()) {
		REAL(munmap);
		size_t whole = len - len % (size_t)page();
		(void)real_munmap(p + whole, (size_t)page());	/* only whole pages mapped */
	}
	if (IS("tail_garbage") && !anon) {
		struct stat st;
		if (fstat(fd, &st) == 0 && st.st_size > off && (size_t)(st.st_size - off) < len &&
		    (st.st_size - off) % page() != 0) {
			size_t from = (size_t)(st.st_size - off), to = from - from % (size_t)page() + (size_t)page();
			if (to > len)
				to = len;
			if (real_mprotect(p, len, PROT_READ | PROT_WRITE) == 0)
				memset(p + from, 'Z', to - from);	/* stale data past the end */
			(void)real_mprotect(p, len, prot);
		}
	}
	if (!anon && nrecs < 64) {
		recs[nrecs].addr = p;
		recs[nrecs].len = len;
		recs[nrecs].fd = fd;
		recs[nrecs].dupfd = dup(fd);
		recs[nrecs].flags = flags;
		recs[nrecs].off = off;
		nrecs++;
	}
	return p;
}

int munmap(void *addr, size_t len)
{
	REAL(munmap);
	const char *m = mutant();
	int i;

	if (IS("tail_written")) {
		for (i = 0; i < nrecs; i++) {
			struct rec *r = &recs[i];
			if (r->addr == addr && (r->flags & MAP_SHARED) && r->dupfd >= 0) {
				size_t n = r->len + (size_t)page() - 1;
				n -= n % (size_t)page();	/* the whole last page, past the end too */
				(void)pwrite(r->dupfd, r->addr, n, r->off);
			}
		}
	}
	return real_munmap(addr, len);
}

int close(int fd)
{
	REAL(close);
	REAL(munmap);
	const char *m = mutant();
	int i;

	if (IS("close_unmaps")) {
		for (i = 0; i < nrecs; i++)
			if (recs[i].fd == fd && recs[i].addr != NULL) {
				(void)real_munmap(recs[i].addr, recs[i].len);	/* the mapping held no reference */
				recs[i].addr = NULL;
			}
	}
	return real_close(fd);
}
