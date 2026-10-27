/*
 * sem_post() returns 0 and adds one to the value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_post.001
 */
#include "semh.h"

int main(void)
{
	sem_t s;
	int r;

	pc_begin("XSH.sem_post.001");
	guard(30);
	new_sem(&s, 0, 0);
	pc_test();
	r = sem_post(&s);
	pc_check(r == 0, "sem_post() returned %d (%s)", r, pc_errname(errno));
	r = sem_post(&s);
	pc_check(r == 0, "second sem_post() returned %d (%s)", r, pc_errname(errno));
	expect_count(&s, 2, "after two posts of value 0");
	pc_check(sem_destroy(&s) == 0, "sem_destroy failed");
	new_sem(&s, 0, 2);
	r = sem_post(&s);
	pc_check(r == 0, "sem_post() on value 2 returned %d", r);
	expect_count(&s, 3, "after a post of value 2");
	pc_pass();
}
