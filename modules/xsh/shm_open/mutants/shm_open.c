/*
 * shm_open.c - planted faults in shm_open(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "shm_open.<fault>" and apply only
 * while PC_PHASE is "test". close() is wrapped for "close_discards".
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
#include <time.h>
#include <unistd.h>

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "shm_open.", 9) != 0)
		return NULL;
	return m + 9;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

static int shm_fds[64];
static int nshm;

/* Names made by "name_per_process", removed when the test process exits. */
static char made[8][128];
static int nmade;
static pid_t maker;

static void unmake(void)
{
	REAL(shm_unlink);
	int i;

	if (getpid() != maker)
		return;
	for (i = 0; i < nmade; i++)
		(void)real_shm_unlink(made[i]);
}

static int exists(const char *name)
{
	REAL(shm_open);
	REAL(close);
	int fd = real_shm_open(name, O_RDONLY, 0);

	if (fd < 0)
		return errno != ENOENT;
	(void)real_close(fd);
	return 1;
}

int shm_open(const char *name, int oflag, mode_t mode)
{
	REAL(shm_open);
	REAL(close);
	const char *m = mutant();
	char buf[128];
	int fd, existed = 0;

	if (m == NULL)
		return real_shm_open(name, oflag, mode);
	if (IS("name_per_process")) {
		snprintf(buf, sizeof buf, "%s.%ld", name, (long)getpid());	/* names are private to a process */
		name = buf;
		if ((oflag & O_CREAT) && nmade < 8) {
			if (nmade == 0) {
				maker = getpid();
				(void)atexit(unmake);
			}
			snprintf(made[nmade++], sizeof made[0], "%s", buf);
		}
	}
	if (IS("rdonly_as_rdwr") && (oflag & O_ACCMODE) == O_RDONLY)
		oflag = (oflag & ~O_ACCMODE) | O_RDWR;
	if (IS("excl_ignored"))
		oflag &= ~O_EXCL;
	if (IS("trunc_ignored"))
		oflag &= ~O_TRUNC;
	if (IS("creat_resets") && (oflag & O_CREAT) && !(oflag & O_EXCL))
		existed = exists(name);
	if (IS("excl_racy") && (oflag & O_CREAT) && (oflag & O_EXCL)) {
		struct timespec ts = { 0, 20000000 };
		if (exists(name)) {
			errno = EEXIST;
			return -1;
		}
		(void)nanosleep(&ts, NULL);	/* check, then create: not atomic */
		oflag &= ~O_EXCL;
	}
	fd = real_shm_open(name, oflag, mode);
	if (fd < 0) {
		if (IS("enoent_creates") && errno == ENOENT && !(oflag & O_CREAT))
			return real_shm_open(name, oflag | O_CREAT, 0600);
		if (IS("eacces_as_eperm") && errno == EACCES)
			errno = EPERM;
		if (IS("emfile_as_enfile") && errno == EMFILE)
			errno = ENFILE;
		return fd;
	}
	if (IS("nonzero_size") && (oflag & O_CREAT) && (oflag & O_EXCL))
		(void)ftruncate(fd, 1);
	if (IS("ignores_umask") && (oflag & O_CREAT) && (oflag & O_EXCL))
		(void)fchmod(fd, mode & 0777);
	if (IS("mode_ignored") && (oflag & O_CREAT) && (oflag & O_EXCL))
		(void)fchmod(fd, 0600);
	if (IS("no_cloexec"))
		(void)fcntl(fd, F_SETFD, 0);
	if (IS("high_fd")) {
		int hi = fcntl(fd, F_DUPFD_CLOEXEC, fd + 3);
		if (hi >= 0) {
			(void)real_close(fd);
			fd = hi;
		}
	}
	if (IS("nonblock"))
		(void)fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
	if (IS("creat_resets") && existed)
		(void)ftruncate(fd, 0);	/* O_CREAT re-creates the object */
	if (nshm < 64)
		shm_fds[nshm++] = fd;
	return fd;
}

int close(int fd)
{
	REAL(close);
	const char *m = mutant();
	int i;

	if (IS("close_discards"))
		for (i = 0; i < nshm; i++)
			if (shm_fds[i] == fd) {
				(void)ftruncate(fd, 0);	/* the object goes with its last descriptor */
				shm_fds[i] = -1;
			}
	return real_close(fd);
}
