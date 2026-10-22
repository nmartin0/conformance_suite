/*
 * close.c - planted faults in close(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef int (*close_fn)(int);

int close(int fd)
{
	static close_fn real;
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");
	int r;

	if (real == NULL)
		real = (close_fn)dlsym(RTLD_NEXT, "close");
	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL ||
	    strncmp(m, "close.", 6) != 0)
		return real(fd);
	m += 6;
	if (strcmp(m, "noop") == 0)
		return 0;
	if (strcmp(m, "always_ok") == 0) {
		(void)real(fd);
		return 0;
	}
	r = real(fd);
	if (strcmp(m, "fail_valid") == 0 && r == 0) {
		errno = EIO;
		return -1;
	}
	if (strcmp(m, "ebadf_as_einval") == 0 && r == -1 && errno == EBADF)
		errno = EINVAL;
	return r;
}
