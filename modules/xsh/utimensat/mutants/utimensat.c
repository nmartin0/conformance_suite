/*
 * utimensat.c - planted faults in utimensat(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "<function>.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static char phase[16];

/* pc.c sets PC_PHASE with setenv(); remember it in this process. */
int setenv(const char *name, const char *value, int overwrite)
{
	static int (*real)(const char *, const char *, int);

	if (real == NULL)
		real = (int (*)(const char *, const char *, int))dlsym(RTLD_NEXT, "setenv");
	if (strcmp(name, "PC_PHASE") == 0 && strlen(value) < sizeof phase)
		strcpy(phase, value);
	return real(name, value, overwrite);
}

static const char *mutant_of(const char *prefix)
{
	const char *m = getenv("PC_MUTANT");
	size_t n = strlen(prefix);

	if (strcmp(phase, "test") != 0 || m == NULL || strncmp(m, prefix, n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)
#define REAL(fn) static __typeof__(fn) *real_##fn; \
	if (real_##fn == NULL) real_##fn = (__typeof__(fn) *)dlsym(RTLD_NEXT, #fn)

#include <sys/time.h>
#include <time.h>

static int fail(int e)
{
	errno = e;
	return -1;
}

static int nsec_ok(long ns)
{
	return ns == UTIME_NOW || ns == UTIME_OMIT || (ns >= 0 && ns < 1000000000L);
}

int utimensat(int fd, const char *path, const struct timespec times[2], int flag)
{
	REAL(utimensat);
	const char *m = mutant_of("utimensat.");
	struct timespec v[2], nowv;
	struct stat st;
	int r, given;

	if (m == NULL)
		return real_utimensat(fd, path, times, flag);
	if (times != NULL) {
		v[0] = times[0];
		v[1] = times[1];
	}
	given = times != NULL && !(v[0].tv_nsec == UTIME_NOW && v[1].tv_nsec == UTIME_NOW) &&
		!(v[0].tv_nsec == UTIME_OMIT && v[1].tv_nsec == UTIME_OMIT);
	if (IS("noop") || (IS("missing_ok") && fstatat(fd, path, &st, 0) != 0 && errno == ENOENT))
		return 0;
	if (IS("dir_slash_refused") && path != NULL && path[0] != '\0' && path[strlen(path) - 1] == '/')
		return fail(ENOTDIR);
	if (IS("null_ignored") && times == NULL)
		return 0;
	if (IS("swaps") && times != NULL) {
		struct timespec x = v[0];
		v[0] = v[1];
		v[1] = x;
	}
	if (IS("rounds_up") && times != NULL) {
		int i;
		for (i = 0; i < 2; i++)
			if (v[i].tv_nsec >= 0 && v[i].tv_nsec < 999999999L)
				v[i].tv_nsec++;
	}
	if (IS("drops_ns") && times != NULL) {
		int i;
		for (i = 0; i < 2; i++)
			if (v[i].tv_nsec >= 0 && v[i].tv_nsec < 1000000000L)
				v[i].tv_nsec -= v[i].tv_nsec % 1000;	/* keeps microseconds only */
	}
	if (IS("max_nsec_rejected") && times != NULL && (v[0].tv_nsec == 999999999L || v[1].tv_nsec == 999999999L))
		return fail(EINVAL);
	if (IS("any_now_ok") && given && (v[0].tv_nsec == UTIME_NOW || v[1].tv_nsec == UTIME_NOW) &&
	    fstatat(fd, path, &st, flag) == 0 && st.st_uid != geteuid() && geteuid() != 0) {
		v[0].tv_nsec = v[1].tv_nsec = UTIME_NOW;	/* treats any UTIME_NOW as the write-access case */
		return real_utimensat(fd, path, v, flag);
	}
	if (IS("now_ignored") && times != NULL) {
		int i;
		for (i = 0; i < 2; i++)
			if (v[i].tv_nsec == UTIME_NOW)
				v[i].tv_nsec = 0;	/* uses tv_sec as given */
	}
	if (IS("omit_ignored") && times != NULL) {
		int i;
		(void)clock_gettime(CLOCK_REALTIME, &nowv);
		for (i = 0; i < 2; i++)
			if (v[i].tv_nsec == UTIME_OMIT)
				v[i] = nowv;
	}
	if (IS("einval_accepted") && times != NULL && (!nsec_ok(v[0].tv_nsec) || !nsec_ok(v[1].tv_nsec)))
		return 0;
	if (IS("nofollow_ignored"))
		flag &= ~AT_SYMLINK_NOFOLLOW;
	if (IS("never_follows"))
		flag |= AT_SYMLINK_NOFOLLOW;
	if (IS("ignores_fd") && path != NULL && path[0] != '/')
		fd = AT_FDCWD;
	if (IS("search_checked") && fd != AT_FDCWD && path != NULL && path[0] != '/' && fstat(fd, &st) == 0 &&
	    S_ISDIR(st.st_mode) && !(st.st_mode & 0111))
		return fail(EACCES);	/* checks search even through an O_SEARCH descriptor */
	if (IS("checks_fd") && fd != AT_FDCWD && fstat(fd, &st) != 0)
		return fail(EBADF);
	if ((IS("bad_fd_as_cwd") || IS("file_fd_as_cwd")) && fd != AT_FDCWD && path != NULL && path[0] != '/') {
		int ok = fstat(fd, &st) == 0;
		if ((IS("bad_fd_as_cwd") && !ok) || (IS("file_fd_as_cwd") && ok && !S_ISDIR(st.st_mode)))
			fd = AT_FDCWD;
	}
	if (IS("perm_owner_only") && !given && !(times != NULL && v[0].tv_nsec == UTIME_OMIT) &&
	    fstatat(fd, path, &st, flag) == 0 && st.st_uid != geteuid() && geteuid() != 0)
		return fail(EACCES);	/* write access no longer enough */
	if (IS("omit_checked") && times != NULL && !given && v[0].tv_nsec == UTIME_OMIT &&
	    fstatat(fd, path, &st, flag) == 0 && st.st_uid != geteuid() && geteuid() != 0)
		return fail(EPERM);
	if (IS("needs_write") && given && geteuid() != 0 && fstatat(fd, path, &st, flag) == 0 && !(st.st_mode & S_IWUSR))
		return fail(EPERM);	/* demands write permission of the owner */
	if (IS("slash_ignored") && path != NULL) {
		static char b[4096];
		size_t n = strlen(path);
		if (n > 1 && n < sizeof b && path[n - 1] == '/') {
			strcpy(b, path);
			b[n - 1] = '\0';
			path = b;
		}
	}
	r = real_utimensat(fd, path, times != NULL ? v : NULL, flag);
	if (r == -1 && IS("enotdir_as_enoent") && errno == ENOTDIR)
		errno = ENOENT;
	if (r == -1 && IS("eloop_as_enoent") && errno == ELOOP)
		errno = ENOENT;
	if (r == -1 && IS("long_as_enoent") && errno == ENAMETOOLONG)
		errno = ENOENT;
	if (r == -1 && IS("eacces_as_enoent") && errno == EACCES)
		errno = ENOENT;
	if (r == -1 && IS("eacces_as_eperm") && errno == EACCES)
		errno = EPERM;
	if (r == -1 && IS("eperm_as_eacces") && errno == EPERM)
		errno = EACCES;
	if (r == 0 && IS("returns_one"))
		return 1;
	return r;
}
