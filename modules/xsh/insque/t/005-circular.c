/*
 * insque() and remque() on a circular queue.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.insque.005
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

	pc_begin("XSH.insque.005");
	a.v = 1;
	b.v = 2;
	c.v = 3;
	a.next = a.prev = &a;
	b.next = b.prev = NULL;
	c.next = c.prev = NULL;
	pc_test();
	insque(&b, &a);
	pc_check(a.next == &b && a.prev == &b, "a ring of two: a's pointers are wrong");
	pc_check(b.next == &a && b.prev == &a, "a ring of two: b's pointers are wrong");
	insque(&c, &a);
	pc_check(a.next == &c && c.next == &b && b.next == &a, "the forward ring is not a, c, b");
	pc_check(a.prev == &b && b.prev == &c && c.prev == &a, "the backward ring is not a, b, c reversed");
	remque(&c);
	pc_check(a.next == &b && b.next == &a, "after removing c the forward ring is not a, b");
	pc_check(a.prev == &b && b.prev == &a, "after removing c the backward ring is not a, b");
	pc_pass();
}
