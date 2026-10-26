/*
 * pcfx.c - fixture faults for ci/mutate.sh's self-test (CI only).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
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

/* "sometimes": the fault is on in every second run, by a flip file the self-test resets. */
static int flip(void)
{
	static int done, on;
	const char *f = getenv("PC_FX_FLIP");
	FILE *fp;
	int c;

	if (done || f == NULL)
		return on;
	done = 1;
	fp = fopen(f, "r+");
	if (fp == NULL)
		return 0;
	c = fgetc(fp);
	on = c == '1';
	rewind(fp);
	fputc(on ? '0' : '1', fp);
	fclose(fp);
	return on;
}

/* Faults act on getppid(): the harness itself uses getpid(). */
pid_t getppid(void)
{
	static pid_t (*real)(void);
	const char *m = mutant_of("pcfx.");

	if (real == NULL)
		real = (pid_t (*)(void))dlsym(RTLD_NEXT, "getppid");
	if (IS("pid_zero"))
		return 0;
	if (IS("harmless"))
		return real();
	if (IS("sometimes") && flip())
		return 0;
	return real();
}
