/*
 * munmap.c - planted faults in munmap(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "munmap.<fault>" and apply only
 * while PC_PHASE is "test". mmap() is wrapped to remember private file
 * mappings for "private_writeback".
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "munmap.", 7) != 0)
		return NULL;
	return m + 7;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

static struct rec {
	char *addr;
	size_t len;
	int dupfd;
	off_t off;
} recs[32];
static int nrecs;

void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off)
{
	REAL(mmap);
	const char *m = mutant();
	char *p = real_mmap(addr, len, prot, flags, fd, off);

	if (IS("private_writeback") && p != MAP_FAILED && (flags & MAP_PRIVATE) && !(flags & MAP_ANONYMOUS) &&
	    nrecs < 32) {
		recs[nrecs].addr = p;
		recs[nrecs].len = len;
		recs[nrecs].dupfd = dup(fd);
		recs[nrecs].off = off;
		nrecs++;
	}
	return p;
}

int munmap(void *addr, size_t len)
{
	REAL(munmap);
	const char *m = mutant();
	long pg = sysconf(_SC_PAGESIZE);
	int r, i;

	if (IS("noop"))
		return 0;
	if (IS("len0_ok") && len == 0)
		return 0;
	if (IS("rounds_down")) {
		len -= len % (size_t)pg;	/* only the whole pages inside the range */
		if (len == 0)
			return 0;
	}
	if (IS("unmapped_einval") && msync(addr, len, MS_ASYNC) == -1 && errno == ENOMEM) {
		errno = EINVAL;	/* treats a range with no mapping as an error */
		return -1;
	}
	if (IS("private_writeback"))
		for (i = 0; i < nrecs; i++)
			if (recs[i].addr == addr && recs[i].dupfd >= 0)
				(void)pwrite(recs[i].dupfd, addr, recs[i].len, recs[i].off);
	r = real_munmap(addr, len);
	if (IS("no_range_check") && r == -1 && errno == EINVAL)
		return 0;
	return r;
}
