/*
 * close.c - planted faults in close(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
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
	if (strcmp(m, "ofd_unlock_each") == 0) {
		/* Drops the description's OFD locks at every close, not the last. */
#ifdef F_OFD_SETLK
		struct flock fl;
		memset(&fl, 0, sizeof fl);
		fl.l_type = F_UNLCK;
		fl.l_whence = SEEK_SET;
		(void)fcntl(fd, F_OFD_SETLK, &fl);
#endif
	}
	if (strcmp(m, "shutdown_each") == 0)
		(void)shutdown(fd, SHUT_RDWR);	/* ends the socket at any close */
	r = real(fd);
	if (strcmp(m, "eagain_nonblock") == 0 && r == 0) {
		errno = EAGAIN;
		return -1;
	}
	if (strcmp(m, "fail_valid") == 0 && r == 0) {
		errno = EIO;
		return -1;
	}
	if (strcmp(m, "ebadf_as_einval") == 0 && r == -1 && errno == EBADF)
		errno = EINVAL;
	return r;
}

#ifdef POSIX_CLOSE_RESTART
typedef int (*posix_close_fn)(int, int);

int posix_close(int fd, int flag)
{
	static posix_close_fn real;
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");

	if (real == NULL)
		real = (posix_close_fn)dlsym(RTLD_NEXT, "posix_close");
	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL ||
	    strncmp(m, "close.", 6) != 0)
		return real(fd, flag);
	m += 6;
	if (strcmp(m, "pc_noop") == 0)
		return 0;
	if (strcmp(m, "pc_einval_open") == 0 && flag != 0 &&
	    flag != POSIX_CLOSE_RESTART) {
		errno = EINVAL;
		return -1;
	}
	return real(fd, flag);
}
#endif
