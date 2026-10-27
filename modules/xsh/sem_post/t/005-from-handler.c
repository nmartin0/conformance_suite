/*
 * sem_post() can be called from a signal-catching function.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_post.005
 */
#include "semh.h"

static sem_t s;
static atomic_int posted, post_result;

static void on_sig(int sig)
{
	(void)sig;
	atomic_store(&post_result, sem_post(&s));
	atomic_store(&posted, 1);
}

int main(void)
{
	struct sigaction sa;
	int r;

	pc_begin("XSH.sem_post.005");
	guard(30);
	new_sem(&s, 0, 0);
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_sig;
	sigemptyset(&sa.sa_mask);
	NEED(sigaction(SIGUSR1, &sa, NULL) == 0, "sigaction failed");
	pc_test();
	r = raise(SIGUSR1);
	pc_check(r == 0, "raise failed");
	pc_check(atomic_load(&posted) == 1, "the handler did not run");
	pc_check(atomic_load(&post_result) == 0, "sem_post() in the handler returned %d", atomic_load(&post_result));
	expect_count(&s, 1, "after a post from a handler");
	pc_pass();
}
