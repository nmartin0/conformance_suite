/*
 * With _XOPEN_SOURCE 800 and _POSIX_C_SOURCE 202405L both defined, XSI and
 * POSIX interfaces are visible and XSI is claimed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.2-compilation-environment.005
 */
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 800
#endif
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 202405L
#endif
#include <fcntl.h>
#include <ftw.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>
#include "pc.h"

#ifndef _XOPEN_UNIX
#error "_XOPEN_UNIX is not defined with _XOPEN_SOURCE 800"
#endif

static int visit(const char *p, const struct stat *s, int f, struct FTW *w)
{
	(void)p; (void)s; (void)f; (void)w;
	return 0;
}

int main(void)
{
	struct timespec ts;
	long r;
	/* The XSI interfaces are named so they are used, not just declared. */
	int (*p_nftw)(const char *, int (*)(const char *, const struct stat *, int, struct FTW *), int, int) = nftw;
	int (*p_shmget)(key_t, size_t, int) = shmget;
	int (*p_msgget)(key_t, int) = msgget;
	int (*p_semget)(key_t, int, int) = semget;

	pc_begin("XSH.2.2-compilation-environment.005");
	pc_test();
	pc_check(p_nftw != NULL && p_shmget != NULL && p_msgget != NULL && p_semget != NULL,
	    "an XSI function has no address");
	pc_check(p_nftw(".", visit, 4, 0) == 0, "nftw(\".\") did not succeed");
	srand48(5);
	r = lrand48();
	pc_check(r >= 0 && r < 2147483648L, "lrand48() returned %ld", r);
	pc_check(strnlen("abcdef", 3) == 3, "strnlen(\"abcdef\", 3) is not 3");
	pc_check(clock_gettime(CLOCK_REALTIME, &ts) == 0, "clock_gettime failed");
	pc_check(O_CLOEXEC != 0, "O_CLOEXEC is zero");
	pc_check(sysconf(_SC_XOPEN_UNIX) > 0, "sysconf(_SC_XOPEN_UNIX) is not positive");
	pc_pass();
}
