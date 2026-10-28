/*
 * With only _XOPEN_SOURCE 800 defined, the POSIX interfaces are visible as well.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.2-compilation-environment.006
 */
#ifdef _POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#endif
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 800
#endif
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>
#include "pc.h"

#if !defined(_POSIX_VERSION) || _POSIX_VERSION < 202405L
#error "_POSIX_VERSION is below 202405L with _XOPEN_SOURCE 800"
#endif
#ifndef _XOPEN_UNIX
#error "_XOPEN_UNIX is not defined with _XOPEN_SOURCE 800"
#endif

int main(void)
{
	struct timespec ts;
	void *m = NULL;
	long r;
	int (*p_shmget)(key_t, size_t, int) = shmget;

	pc_begin("XSH.2.2-compilation-environment.006");
	pc_test();
	pc_check(strnlen("abcdef", 3) == 3, "strnlen(\"abcdef\", 3) is not 3");
	pc_check(clock_gettime(CLOCK_MONOTONIC, &ts) == 0, "clock_gettime failed");
	pc_check(posix_memalign(&m, 64, 100) == 0 && m != NULL && ((unsigned long)m % 64) == 0,
	    "posix_memalign failed");
	free(m);
	pc_check(O_CLOEXEC != 0, "O_CLOEXEC is zero");
	srand48(9);
	r = lrand48();
	pc_check(r >= 0, "lrand48() returned %ld", r);
	pc_check(p_shmget != NULL, "shmget has no address");
	pc_pass();
}
