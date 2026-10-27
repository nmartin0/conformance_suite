/*
 * ftrylockfile() on a stream no thread owns acquires it and returns zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.flockfile.001
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* A stream on path in mode (a precondition). */
static FILE *open_as(const char *path, const char *mode)
{
	FILE *f = fopen(path, mode);

	NEED(f != NULL, "fopen(%s, %s) failed: %s", path, mode, pc_errname(errno));
	return f;
}

struct targ {
	FILE *f;
	int result;
	atomic_int flag;
};

static void *try_thread(void *p)
{
	struct targ *a = p;

	a->result = ftrylockfile(a->f);
	if (a->result == 0)
		funlockfile(a->f);
	return NULL;
}

/* ftrylockfile(f) as called by another thread (it never waits): its return value. */
static int try_elsewhere(FILE *f)
{
	struct targ a;
	pthread_t t;
	int r;

	a.f = f;
	a.result = -1;
	atomic_init(&a.flag, 0);
	r = pthread_create(&t, NULL, try_thread, &a);
	NEED(r == 0, "pthread_create failed: %s", pc_errname(r));
	r = pthread_join(t, NULL);
	NEED(r == 0, "pthread_join failed: %s", pc_errname(r));
	return a.result;
}

int main(void)
{
	pc_begin("XSH.flockfile.001");
	{
		FILE *f;
		int r;

		pc_test();
		f = open_as("f", "w+");
		r = ftrylockfile(f);
		pc_check(r == 0, "ftrylockfile() on a stream no thread owns returned %d, expected 0", r);
		funlockfile(f);
		r = try_elsewhere(f);
		pc_check(r == 0, "ftrylockfile() by another thread after funlockfile() returned %d, expected 0", r);
		(void)fclose(f);
	}
	pc_pass();
}
