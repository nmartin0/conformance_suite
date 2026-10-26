/*
 * mprotect.c - planted faults in mprotect(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "mprotect.<fault>" and apply only
 * while PC_PHASE is "test". mmap() is wrapped to remember private
 * mappings of read-only descriptors for "private_eacces".
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "mprotect.", 9) != 0)
		return NULL;
	return m + 9;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

static char *ro_private[32];
static int nro;

void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off)
{
	REAL(mmap);
	const char *m = mutant();
	char *p = real_mmap(addr, len, prot, flags, fd, off);

	if (IS("private_eacces") && p != MAP_FAILED && (flags & MAP_PRIVATE) && !(flags & MAP_ANONYMOUS) &&
	    (fcntl(fd, F_GETFL) & O_ACCMODE) == O_RDONLY && nro < 32)
		ro_private[nro++] = p;
	return p;
}

int mprotect(void *addr, size_t len, int prot)
{
	REAL(mprotect);
	const char *m = mutant();
	long pg = sysconf(_SC_PAGESIZE);
	int r, i;

	if (IS("noop"))
		return 0;
	if (IS("none_as_read") && prot == PROT_NONE)
		prot = PROT_READ;
	if (IS("rounds_down")) {
		len -= len % (size_t)pg;	/* only the whole pages inside the range */
		if (len == 0)
			return 0;
	}
	if (IS("write_only_enotsup") && prot == PROT_WRITE) {
		errno = ENOTSUP;
		return -1;
	}
	if (IS("private_eacces") && (prot & PROT_WRITE))
		for (i = 0; i < nro; i++)
			if (ro_private[i] == addr) {
				errno = EACCES;	/* checks the descriptor as for MAP_SHARED */
				return -1;
			}
	r = real_mprotect(addr, len, prot);
	if (r == -1 && errno == EACCES) {
		if (IS("eacces_as_eperm"))
			errno = EPERM;
		if (IS("eacces_ignored"))
			return 0;
	}
	if (IS("unmapped_ok") && r == -1 && errno == ENOMEM)
		return 0;
	return r;
}
