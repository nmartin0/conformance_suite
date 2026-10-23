/*
 * chown.c - planted faults in chown(), for mutation checks (CI only).
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

static int has_slash_end(const char *p)
{
	size_t n = strlen(p);

	return n > 1 && p[n - 1] == '/';
}

int chown(const char *path, uid_t owner, gid_t group)
{
	REAL(chown);
	const char *m = mutant_of("chown.");
	struct stat st;
	int r;

	if (m == NULL)
		return real_chown(path, owner, group);
	if (IS("noop"))
		return 0;
	if (IS("ctime_kept") && stat(path, &st) == 0 && owner == st.st_uid && group == st.st_gid)
		return 0;	/* skips a call that changes no ID, so st_ctim is not marked */
	if (IS("minus_one_group_used") && group == (gid_t)-1)
		group = 0;
	if (IS("foreign_group_ok") && geteuid() != 0 && group != (gid_t)-1 && group != getegid() && stat(path, &st) == 0 &&
	    st.st_uid == geteuid())
		return 0;	/* pretends any group may be set */
	if (IS("partial_change") && geteuid() != 0 && owner != (uid_t)-1 && group != (gid_t)-1 && stat(path, &st) == 0 &&
	    owner != st.st_uid) {
		(void)real_chown(path, (uid_t)-1, group);	/* applies the group, then fails */
		return fail(EPERM);
	}
	if (IS("swaps"))
		return real_chown(path, (uid_t)group, (gid_t)owner);
	if (IS("minus_one_used") && owner == (uid_t)-1)
		owner = 0;
	if (IS("owner_ignored") && stat(path, &st) == 0 && st.st_uid != geteuid())
		return 0;	/* reports success for a file the caller does not own */
	if (IS("restriction_ignored") && geteuid() != 0 && owner != (uid_t)-1 && stat(path, &st) == 0 && owner != st.st_uid)
		return 0;	/* pretends a file can be given away */
	if (IS("owner_refused") && geteuid() != 0)
		return fail(EPERM);
	if (IS("missing_ok") && stat(path, &st) != 0)
		return 0;
	if (IS("no_follow") && lstat(path, &st) == 0 && S_ISLNK(st.st_mode))
		return lchown(path, owner, group);
	if (IS("slash_ignored") && has_slash_end(path)) {
		static char b[4096];
		if (strlen(path) < sizeof b) {
			strcpy(b, path);
			b[strlen(b) - 1] = '\0';
			path = b;
		}
	}
	if (IS("keeps_setid") && geteuid() != 0 && stat(path, &st) == 0 && (st.st_mode & S_ISUID)) {
		mode_t keep = st.st_mode & 07777;
		r = real_chown(path, owner, group);
		(void)chmod(path, keep);	/* puts the set-ID bits back */
		return r;
	}
	r = real_chown(path, owner, group);
	if (r == -1 && IS("eacces_as_eperm") && errno == EACCES)
		errno = EPERM;
	if (r == -1 && IS("ebadf_as_einval") && errno == EBADF)
		errno = EINVAL;
	if (r == -1 && IS("enoent_as_enotdir") && errno == ENOENT)
		errno = ENOTDIR;
	if (r == -1 && IS("enotdir_as_enoent") && errno == ENOTDIR)
		errno = ENOENT;
	if (r == -1 && IS("eloop_as_enoent") && errno == ELOOP)
		errno = ENOENT;
	if (r == -1 && IS("long_as_enoent") && errno == ENAMETOOLONG)
		errno = ENOENT;
	if (r == 0 && IS("returns_one"))
		return 1;
	return r;
}
