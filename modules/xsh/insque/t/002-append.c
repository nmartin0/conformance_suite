/*
 * insque() after the last element.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.insque.002
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
	struct q e[5];
	int i;

	pc_begin("XSH.insque.002");
	for (i = 0; i < 5; i++) {
		e[i].next = e[i].prev = (struct q *)&e[4 - i];
		e[i].v = i;
	}
	pc_test();
	insque(&e[0], NULL);
	for (i = 1; i < 5; i++) {
		insque(&e[i], &e[i - 1]);
		pc_check(e[i - 1].next == &e[i], "after inserting %d, element %d does not lead to it", i, i - 1);
		pc_check(e[i].prev == &e[i - 1], "after inserting %d, its backward pointer is wrong", i);
		pc_check(e[i].next == NULL, "after inserting %d at the end, its forward pointer is not null", i);
	}
	pc_check(e[0].prev == NULL, "the first element's backward pointer is not null");
	for (i = 0; i < 4; i++) {
		pc_check(e[i].next == &e[i + 1], "forward link %d is wrong", i);
		pc_check(e[i + 1].prev == &e[i], "backward link %d is wrong", i + 1);
	}
	pc_pass();
}
