/*
 * pthread_kill() rejects a signal number that does not exist.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_kill.003
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>

int main(void)
{
	static const int bad[] = { -1, INT_MAX };
	unsigned i;
	int r;

	pc_begin("XSH.pthread_kill.003");
	pc_test();
	for (i = 0; i < 2; i++) {
		r = pthread_kill(pthread_self(), bad[i]);
		pc_check(r == EINVAL, "pthread_kill(self, %d) returned %s, expected EINVAL", bad[i], r == 0 ? "0" : pc_errname(r));
	}
	pc_pass();
}
