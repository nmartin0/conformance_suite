/*
 * dlx.h - helpers for the dynamic linking tests (shared, keep identical).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * The tests use only the running program and the objects it was started
 * with. The function getpid() is the probe: the program refers to it. The
 * page does not say which symbols the program image exports, so a probe that
 * is not found is UNRESOLVED, never FAIL.
 */
#ifndef DLX_H
#define DLX_H

#include "pc.h"
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define DL_MISSING "/nonexistent-pc-dl/none.so"

/* Calls the function whose address dlsym() gave; the program's own getpid() is what it must return. */
static inline pid_t dlx_getpid(void *sym)
{
	pid_t (*fn)(void);

	memcpy(&fn, &sym, sizeof fn);
	return fn();
}

#ifdef DLX_OBJECT
/* Setup: the pathname of the object that defines getpid(), as dladdr() reports it. */
static inline void dlx_object(char *path, size_t n)
{
	void *h, *s;
	Dl_info_t info;

	h = dlopen(NULL, RTLD_NOW);
	pc_check(h != NULL, "dlopen(NULL) failed");
	s = dlsym(h, "getpid");
	pc_check(s != NULL, "dlsym() did not find getpid");
	memset(&info, 0, sizeof info);
	pc_check(dladdr(s, &info) != 0, "dladdr() did not know the address of getpid");
	pc_check(info.dli_fname != NULL && info.dli_fname[0] != '\0', "dladdr() gave no object pathname");
	snprintf(path, n, "%s", info.dli_fname);
	(void)dlclose(h);
}
#endif

/* A string from dlerror() is non-empty text without a trailing newline. */
static inline int dlx_text_ok(const char *s)
{
	size_t n;

	if (s == NULL)
		return 0;
	n = strlen(s);
	return n > 0 && s[n - 1] != '\n';
}

#endif
