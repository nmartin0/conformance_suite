/*
 * strerror_l() in another thread does not overwrite strerror()'s string.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strerror.007
 * MUTANT    strerror.l_shares_buffer
 * VARIANT   strerror.global_buf
 */
#include "pc.h"
#include <errno.h>
#include <locale.h>
#include <pthread.h>
#include <string.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static locale_t loc;

static void *other(void *arg)
{
	(void)arg;
	(void)strerror_l(EINVAL, loc);
	(void)strerror_l(EACCES, loc);
	return NULL;
}

int main(void)
{
	static char copy[4096];
	const char *s;
	pthread_t t;

	pc_begin("XSH.strerror.007");
	loc = newlocale(LC_ALL_MASK, "POSIX", (locale_t)0);
	pc_check(loc != (locale_t)0, "newlocale(POSIX) failed: %s", pc_errname(errno));
	pc_test();
	s = strerror(ENOENT);
	pc_check(s != NULL, "strerror(ENOENT) returned a null pointer");
	if (strlen(s) >= sizeof copy)
		pc_unresolved("the message is longer than %zu bytes", sizeof copy - 1);
	strcpy(copy, s);
	NEED(pthread_create(&t, NULL, other, NULL) == 0, "pthread_create failed");
	NEED(pthread_join(t, NULL) == 0, "pthread_join failed");
	pc_check(strcmp(s, copy) == 0, "the strerror(ENOENT) string changed from \"%s\" to \"%s\"", copy, s);
	pc_pass();
}
