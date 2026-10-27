/*
 * insque() with a null predecessor starts a linear queue.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.insque.001
 */
#include "pc.h"
#include <search.h>
#include <stdlib.h>

struct q {
	struct q *next, *prev;
	int v;
};

int main(void)
{
	struct q e;

	pc_begin("XSH.insque.001");
	e.next = (struct q *)&e;
	e.prev = (struct q *)&e.v;
	e.v = 7;
	pc_test();
	insque(&e, NULL);
	pc_check(e.next == NULL, "the forward pointer is %p, expected null", (void *)e.next);
	pc_check(e.prev == NULL, "the backward pointer is %p, expected null", (void *)e.prev);
	pc_check(e.v == 7, "the application data changed");
	pc_pass();
}
