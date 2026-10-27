/*
 * mqd_t is not an array type: a function can return one, take one by
 * value, and objects of the type can be assigned.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.mqueue.h.001
 */
#include "pc.h"
#include <mqueue.h>
#include <string.h>

/* A function can return an mqd_t and take one by value, and objects of the type can be assigned: an array type cannot do any of this. */
static mqd_t pass_through(mqd_t d)
{
	return d;
}

int main(void)
{
	mqd_t a, b;

	pc_begin("XBD.mqueue.h.001");
	pc_test();
	memset(&a, 0, sizeof a);
	b = pass_through(a);
	a = b;
	pc_check(memcmp(&a, &b, sizeof a) == 0, "assignment of an mqd_t did not copy it");
	pc_pass();
}
