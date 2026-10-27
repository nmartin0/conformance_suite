/*
 * insque() between two elements.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.insque.003
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
	struct q a, b, c;

	pc_begin("XSH.insque.003");
	a.v = 1;
	b.v = 2;
	c.v = 3;
	c.next = c.prev = &a;
	pc_test();
	insque(&a, NULL);
	insque(&b, &a);
	insque(&c, &a);
	pc_check(a.prev == NULL, "a's backward pointer is not null");
	pc_check(a.next == &c, "c does not follow a");
	pc_check(c.prev == &a, "c's backward pointer is not a");
	pc_check(c.next == &b, "b does not follow c");
	pc_check(b.prev == &c, "b's backward pointer is not c");
	pc_check(b.next == NULL, "b's forward pointer is not null");
	pc_pass();
}
