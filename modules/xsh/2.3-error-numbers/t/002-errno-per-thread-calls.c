/*
 * Function calls and assignments in one thread do not affect errno in another.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.3-error-numbers.002
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
static int go, done;
static int own_bad;
static const char *own_what;

static int get(const int *flag)
{
	int v;

	(void)pthread_mutex_lock(&mu);
	v = *flag;
	(void)pthread_mutex_unlock(&mu);
	return v;
}

static void set(int *flag)
{
	(void)pthread_mutex_lock(&mu);
	*flag = 1;
	(void)pthread_mutex_unlock(&mu);
}

static void *other(void *arg)
{
	int i;
	char b[4];
	char *end;

	(void)arg;
	while (!get(&go))
		;
	for (i = 0; i < 3000 && !own_bad; i++) {
		errno = 0;
		if (open("pc-no-such-file", O_RDONLY) != -1 || errno != ENOENT) {
			own_bad = 1;
			own_what = "open of a missing file";
		}
		if (close(-1) != -1 || errno != EBADF) {
			own_bad = 1;
			own_what = "close(-1)";
		}
		if (read(-1, b, 1) != -1 || errno != EBADF) {
			own_bad = 1;
			own_what = "read(-1)";
		}
		if (strtol("99999999999999999999999999", &end, 10) != LONG_MAX || errno != ERANGE) {
			own_bad = 1;
			own_what = "strtol of a huge number";
		}
		errno = EPERM;
		if (errno != EPERM) {
			own_bad = 1;
			own_what = "reading back an assigned errno";
		}
		errno = 0;
		if (errno != 0) {
			own_bad = 1;
			own_what = "reading back errno assigned 0";
		}
	}
	set(&done);
	return NULL;
}

static void on_alarm(int s)
{
	(void)s;
	pc_fail("the test timed out");
}

int main(void)
{
	pthread_t th;
	int bad = 0;
	long spins = 0;

	pc_begin("XSH.2.3-error-numbers.002");
	(void)signal(SIGALRM, on_alarm);
	pc_test();
	(void)alarm(20);
	if (pthread_create(&th, NULL, other, NULL) != 0)
		pc_unresolved("pthread_create failed");
	errno = EDOM;
	set(&go);
	errno = EDOM;	/* a library call of this thread may change its own errno */
	for (;;) {
		if (errno != EDOM) {
			bad = 1;
			break;
		}
		spins++;
		if (get(&done))
			break;
		errno = EDOM;
	}
	(void)pthread_join(th, NULL);
	pc_check(!bad, "errno of the first thread changed while another thread made calls");
	pc_check(!own_bad, "the second thread saw a wrong errno after %s", own_what);
	if (spins == 0)
		pc_unresolved("the first thread never looked at errno while the other ran");
	pc_pass();
}
