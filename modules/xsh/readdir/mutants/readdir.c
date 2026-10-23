/*
 * readdir.c - planted faults in readdir(), for mutation checks (CI only).
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

static int fail(int e)
{
	errno = e;
	return -1;
}

static void *fail_p(int e)
{
	errno = e;
	return NULL;
}

struct dirent *readdir(DIR *d)
{
	REAL(readdir);
	const char *m = mutant_of("readdir.");
	static struct dirent *last;
	static DIR *last_dir;
	static union { struct dirent d; char pad[sizeof(struct dirent) + 512]; } own;
	struct dirent *e;

	if (m == NULL)
		return real_readdir(d);
	if (IS("skips_first") && last_dir != d) {
		last_dir = d;
		do
			e = real_readdir(d);
		while (e != NULL && e->d_name[0] == '.');	/* the first name that is not dot or dot-dot is lost */
		return real_readdir(d);
	}
	if (IS("dup_entry")) {
		if (last != NULL && last_dir == d && last->d_name[0] != '.') {
			e = last;
			last = NULL;
			return e;	/* the same entry once more */
		}
		last_dir = d;
		last = e = real_readdir(d);
		return e;
	}
	e = real_readdir(d);
	if (IS("errno_at_end") && e == NULL)
		errno = ENOENT;
	if (e == NULL)
		return NULL;
	if (IS("mangles_name") && (unsigned char)e->d_name[0] >= 0x80) {
		memcpy(&own.d, e, sizeof own.d);
		snprintf(own.d.d_name, 256, "?%s", e->d_name + 1);
		return &own.d;
	}
	if (IS("wrong_ino") && e->d_name[0] != '.') {
		memcpy(&own.d, e, sizeof own.d);
		snprintf(own.d.d_name, 256, "%s", e->d_name);
		own.d.d_ino = e->d_ino + 1;
		return &own.d;
	}
	if (IS("follows_ino") && e->d_name[0] != '.') {
		struct stat st;
		int fd = dirfd(d);
		memcpy(&own.d, e, sizeof own.d);
		snprintf(own.d.d_name, 256, "%s", e->d_name);
		if (fd >= 0 && fstatat(fd, e->d_name, &st, 0) == 0)
			own.d.d_ino = st.st_ino;	/* the target's serial number, not the link's */
		return &own.d;
	}
	if (IS("shared_buffer")) {
		memcpy(&own.d, e, sizeof own.d);
		snprintf(own.d.d_name, 256, "%s", e->d_name);
		return &own.d;	/* one buffer for every stream */
	}
	return e;
}
