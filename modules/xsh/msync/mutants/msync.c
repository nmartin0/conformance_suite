/*
 * msync.c - planted faults in msync(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "msync.<fault>" and apply only
 * while PC_PHASE is "test". mmap() is wrapped to remember file mappings
 * (a duplicate descriptor and the file's timestamps when it was mapped).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "msync.", 6) != 0)
		return NULL;
	return m + 6;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

static struct rec {
	char *addr;
	size_t len;
	int dupfd, flags;
	off_t off;
	struct timespec times[2];
} recs[32];
static int nrecs;

void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off)
{
	REAL(mmap);
	const char *m = mutant();
	char *p = real_mmap(addr, len, prot, flags, fd, off);
	struct stat st;

	if (m != NULL && p != MAP_FAILED && !(flags & MAP_ANONYMOUS) && nrecs < 32 && fstat(fd, &st) == 0) {
		recs[nrecs].addr = p;
		recs[nrecs].len = len;
		recs[nrecs].dupfd = dup(fd);
		recs[nrecs].flags = flags;
		recs[nrecs].off = off;
		recs[nrecs].times[0] = st.st_atim;
		recs[nrecs].times[1] = st.st_mtim;
		nrecs++;
	}
	return p;
}

static struct rec *find(void *addr)
{
	int i;

	for (i = 0; i < nrecs; i++)
		if ((char *)addr >= recs[i].addr && (char *)addr < recs[i].addr + recs[i].len)
			return &recs[i];
	return NULL;
}

int msync(void *addr, size_t len, int flags)
{
	REAL(msync);
	const char *m = mutant();
	struct rec *r;
	int ret;

	if (m == NULL)
		return real_msync(addr, len, flags);
	if ((IS("async_einval") && (flags & MS_ASYNC)) || (IS("invalidate_einval") && (flags & MS_INVALIDATE))) {
		errno = EINVAL;
		return -1;
	}
	if (IS("both_ok") && (flags & MS_ASYNC) && (flags & MS_SYNC))
		flags &= ~MS_ASYNC;
	if (IS("none_ok") && !(flags & (MS_ASYNC | MS_SYNC)))
		flags |= MS_ASYNC;	/* a missing MS_ASYNC or MS_SYNC is taken as MS_ASYNC */
	if (IS("private_writeback") && (r = find(addr)) != NULL && (r->flags & MAP_PRIVATE))
		(void)pwrite(r->dupfd, r->addr, r->len, r->off);	/* writes private data out */
	ret = real_msync(addr, len, flags);
	if (IS("unmapped_ok") && ret == -1 && errno == ENOMEM)
		return 0;
	if (IS("ebusy_ignored") && ret == -1 && errno == EBUSY)
		return real_msync(addr, len, flags & ~MS_INVALIDATE);
	if (IS("mtime_kept") && ret == 0 && (r = find(addr)) != NULL && (r->flags & MAP_SHARED))
		(void)futimens(r->dupfd, r->times);	/* stores through mappings never mark the file */
	return ret;
}
