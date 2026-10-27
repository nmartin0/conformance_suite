/*
 * remque() unlinks an element.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.insque.004
 */
#include "pc.h"
#include <search.h>
#include <stdlib.h>

struct q {
	struct q *next, *prev;
	int v;
};

static void build(struct q *e)
{
	int i;

	for (i = 0; i < 4; i++)
		e[i].v = i;
	insque(&e[0], NULL);
	for (i = 1; i < 4; i++)
		insque(&e[i], &e[i - 1]);
}

int main(void)
{
	struct q e[4];

	pc_begin("XSH.insque.004");
	pc_test();
	/* middle */
	build(e);
	remque(&e[1]);
	pc_check(e[0].next == &e[2] && e[2].prev == &e[0], "removing the second element did not join its neighbours");
	pc_check(e[2].next == &e[3] && e[3].prev == &e[2], "removing the second element disturbed the rest");
	pc_check(e[0].prev == NULL && e[3].next == NULL, "the ends are not null after removing a middle element");
	/* first */
	build(e);
	remque(&e[0]);
	pc_check(e[1].prev == NULL, "after removing the first element the new first has a backward pointer");
	pc_check(e[1].next == &e[2] && e[2].prev == &e[1] && e[2].next == &e[3] && e[3].prev == &e[2],
	    "removing the first element disturbed the rest");
	pc_check(e[3].next == NULL, "the last forward pointer is not null");
	/* last */
	build(e);
	remque(&e[3]);
	pc_check(e[2].next == NULL, "after removing the last element the new last has a forward pointer");
	pc_check(e[0].next == &e[1] && e[1].prev == &e[0] && e[1].next == &e[2] && e[2].prev == &e[1],
	    "removing the last element disturbed the rest");
	pc_check(e[0].prev == NULL, "the first backward pointer is not null");
	pc_pass();
}
