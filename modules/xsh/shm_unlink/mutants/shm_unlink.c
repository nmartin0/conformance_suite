/*
 * shm_unlink.c - planted faults in shm_unlink(), for mutation checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "shm_unlink.<fault>" and apply only
 * while PC_PHASE is "test".
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

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "shm_unlink.", 11) != 0)
		return NULL;
	return m + 11;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

int shm_unlink(const char *name)
{
	REAL(shm_unlink);
	const char *m = mutant();
	int r;

	if (IS("noop"))
		return 0;
	if (IS("short_limit") && strlen(name) > 100) {
		errno = ENAMETOOLONG;	/* a shorter limit than shm_open()'s */
		return -1;
	}
	if (IS("truncates")) {
		int fd = shm_open(name, O_RDWR, 0);
		if (fd >= 0) {
			(void)ftruncate(fd, 0);	/* frees the contents at once */
			(void)close(fd);
		}
	}
	r = real_shm_unlink(name);
	if (IS("enoent_ok") && r == -1 && errno == ENOENT)
		return 0;
	if (IS("eacces_as_eperm") && r == -1 && errno == EACCES)
		errno = EPERM;
	return r;
}
