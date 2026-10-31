/*
 * pcfz.c - fixture fault for ci/mutate.sh's self-test (CI only).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * The header redirects pcfz_pid to another symbol, whose name this file
 * must not know: the driver passes it as PC_SYM_pcfz_pid_1 after reading
 * what the probe object references. Without the macro this does not build.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#ifndef PC_SYM_pcfz_pid_1
#error the driver did not name the symbol the header binds pcfz_pid to
#endif

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

pid_t pcfz_entry(void) __asm__(PC_SYM_pcfz_pid_1);

pid_t pcfz_entry(void)
{
	static pid_t (*real)(void);
	const char *m = getenv("PC_MUTANT");

	if (real == NULL)
		real = (pid_t (*)(void))dlsym(RTLD_NEXT, PC_SYM_pcfz_pid_1);
	if (strcmp(phase, "test") == 0 && m != NULL && strcmp(m, "pcfz.zero") == 0)
		return 0;
	return real();
}
