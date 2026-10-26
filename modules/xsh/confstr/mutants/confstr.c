/*
 * confstr.c - planted faults in confstr(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "confstr.<fault>" and apply only
 * while PC_PHASE is "test" (followed through pc.c's setenv() calls).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char phase[16];

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

static int is_v7(int name)
{
	switch (name) {
#ifdef _CS_POSIX_V7_ILP32_OFF32_CFLAGS
	case _CS_POSIX_V7_ILP32_OFF32_CFLAGS: case _CS_POSIX_V7_ILP32_OFF32_LDFLAGS: case _CS_POSIX_V7_ILP32_OFF32_LIBS:
	case _CS_POSIX_V7_LP64_OFF64_CFLAGS: case _CS_POSIX_V7_LP64_OFF64_LDFLAGS: case _CS_POSIX_V7_LP64_OFF64_LIBS:
		return 1;
#endif
	}
	return 0;
}

/* Copies s into buf as a correct confstr() would. */
static size_t give(const char *s, char *buf, size_t len)
{
	size_t n = strlen(s) + 1;

	if (len > 0) {
		size_t k = n < len ? n : len;

		memcpy(buf, s, k);
		buf[k - 1] = '\0';
	}
	return n;
}

size_t confstr(int name, char *buf, size_t len)
{
	static size_t (*real)(int, char *, size_t);
	const char *m = mutant_of("confstr.");
	char full[8192];
	size_t n;
	int e = errno;

	if (real == NULL)
		real = (size_t (*)(int, char *, size_t))dlsym(RTLD_NEXT, "confstr");
	if (m == NULL)
		return real(name, buf, len);
	if (IS("drops_v7") && is_v7(name)) {
		errno = EINVAL;
		return 0;
	}
#ifdef _CS_V7_ENV
	if (IS("no_value_sets_errno") && name == _CS_V7_ENV) {
		errno = ENOENT;	/* "no value" reported with an error number */
		return 0;
	}
#endif
	if (IS("path_empty") && name == _CS_PATH)
		return give("", buf, len);
	if (IS("path_wrong_dir") && name == _CS_PATH)
		return give("/pc-no-such-directory", buf, len);
#ifdef _CS_V8_ENV
	if (IS("env_no_value") && name == _CS_V8_ENV)
		return 0;
	if (IS("env_bare_name") && name == _CS_V8_ENV)
		return give("POSIXLY_CORRECT=1 PCX_BARE", buf, len);
#endif
	n = real(name, full, sizeof full);
	if (n == 0 || n > sizeof full) {
		if (n == 0 && errno == EINVAL) {
			if (IS("invalid_no_errno")) {
				errno = e;
				return 0;
			}
			if (IS("invalid_wrong_errno")) {
				errno = ENOSYS;
				return 0;
			}
		}
		return real(name, buf, len);
	}
	if (IS("length_without_null")) {
		give(full, buf, len);
		return n - 1;
	}
	if (IS("null_buffer_zero") && buf == NULL && len == 0)
		return 0;
	if (IS("no_terminator") && len > 0 && n > len) {
		memcpy(buf, full, len);	/* strncpy()-like: no room left for the null byte */
		return n;
	}
	if (IS("ignores_len") && len > 0) {
		memcpy(buf, full, n);
		return n;
	}
	if (IS("returns_copied") && len > 0 && n > len) {
		give(full, buf, len);
		return len;
	}
	return real(name, buf, len);
}
