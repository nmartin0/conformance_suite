/*
 * realpath.c - planted faults in realpath(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "realpath.<fault>" and apply only
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


#include <limits.h>
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/* Into out: the working directory joined to name (if relative). */
static int absolute(const char *name, char *out, size_t size)
{
	char cwd[PATH_MAX];

	if (name[0] == '/')
		return snprintf(out, size, "%s", name) < (int)size ? 0 : -1;
	if (getcwd(cwd, sizeof cwd) == NULL)
		return -1;
	return snprintf(out, size, "%s/%s", cwd, name) < (int)size ? 0 : -1;
}

/* Removes dot, dot-dot and empty components of the absolute name p, by text only. */
static void lexical(char *p)
{
	char out[PATH_MAX];
	size_t o = 0, i = 0, s;

	while (p[i] != '\0') {
		while (p[i] == '/')
			i++;
		s = i;
		while (p[i] != '\0' && p[i] != '/')
			i++;
		if (i == s)
			break;
		if (i - s == 1 && p[s] == '.')
			continue;
		if (i - s == 2 && p[s] == '.' && p[s + 1] == '.') {
			while (o > 0 && out[o - 1] != '/')
				o--;
			if (o > 0)
				o--;
			continue;
		}
		out[o++] = '/';
		memcpy(out + o, p + s, i - s);
		o += i - s;
	}
	if (o == 0)
		out[o++] = '/';
	out[o] = '\0';
	strcpy(p, out);
}

static char *give(const char *s, char *resolved)
{
	char *r = resolved ? resolved : malloc(strlen(s) + 1);

	if (r == NULL)
		return NULL;
	strcpy(r, s);
	return r;
}

char *realpath(const char *restrict name, char *restrict resolved)
{
	const char *m = mutant_of("realpath.");
	static char stat_buf[PATH_MAX];
	char t[PATH_MAX], b[PATH_MAX];
	struct stat st;
	char *r;
	ssize_t n;
	int e;
	REAL(realpath);

	if (m == NULL)
		return real_realpath(name, resolved);
	if (IS("null_enoent") && name == NULL) {
		errno = ENOENT;
		return NULL;
	}
	if (name == NULL)
		return real_realpath(name, resolved);
	if (IS("null_einval") && resolved == NULL) {
		errno = EINVAL;	/* the old XSI rule for a null resolved_name */
		return NULL;
	}
	if (IS("null_static") && resolved == NULL)
		return real_realpath(name, stat_buf);
	if (IS("ignores_buffer") && resolved != NULL) {
		r = real_realpath(name, stat_buf);
		return r;
	}
	if (IS("keeps_dots"))
		return absolute(name, t, sizeof t) == 0 ? give(t, resolved) : NULL;
	if (IS("relative") && name[0] != '/' && name[0] != '\0') {
		snprintf(t, sizeof t, "/%s", name);	/* tidies the text but does not anchor it */
		lexical(t);
		return give(t + 1, resolved);
	}
	if (IS("lexical")) {
		if (absolute(name, t, sizeof t) != 0)
			return NULL;
		lexical(t);
		return give(t, resolved);
	}
	if (IS("link_rel_cwd") && lstat(name, &st) == 0 && S_ISLNK(st.st_mode) &&
	    (n = readlink(name, t, sizeof t - 1)) > 0) {
		t[n] = '\0';
		if (t[0] != '/')
			return real_realpath(t, resolved);	/* takes the contents from the working directory */
	}
	if (IS("empty_cwd") && name[0] == '\0')
		return real_realpath(".", resolved);
	if (IS("slash_ignored")) {
		size_t k = strlen(name);
		while (k > 1 && name[k - 1] == '/')
			k--;
		if (k < sizeof b) {
			memcpy(b, name, k);
			b[k] = '\0';
			name = b;
		}
	}
	if (IS("long_truncated") && strlen(name) < sizeof b) {
		const char *s = strrchr(name, '/');
		size_t at = s ? (size_t)(s - name) + 1 : 0;
		if (strlen(name) - at > 255) {
			memcpy(b, name, at + 255);	/* keeps {NAME_MAX} bytes, as without _POSIX_NO_TRUNC */
			b[at + 255] = '\0';
			name = b;
		}
	}
	r = real_realpath(name, resolved);
	e = errno;
	if (r == NULL && IS("missing_ok") && e == ENOENT && absolute(name, t, sizeof t) == 0) {
		lexical(t);	/* resolves what exists and keeps the rest */
		return give(t, resolved);
	}
	if (r == NULL && IS("enotdir_as_einval") && e == ENOTDIR)
		errno = EINVAL;
	if (r == NULL && IS("eloop_as_enametoolong") && e == ELOOP)
		errno = ENAMETOOLONG;
	if (r == NULL && IS("eacces_as_enoent") && e == EACCES)
		errno = ENOENT;
	return r;
}
