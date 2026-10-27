/*
 * An fd_set can be copied by assignment, as a structure can, and the copy
 * keeps its own members apart from the original, with the first, middle
 * and last descriptors (0, FD_SETSIZE/2, FD_SETSIZE-1) usable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_select.h.002
 */
#include "pc.h"
#include <sys/select.h>

int main(void)
{
	fd_set a, b;
	int ok;

	pc_begin("XBD.sys_select.h.002");
#ifndef FD_SETSIZE
	pc_unresolved("FD_SETSIZE is missing (the header check reports it)");
#else
	const int last = (FD_SETSIZE) - 1;
	const int mid = (FD_SETSIZE) / 2;

	pc_test();
	FD_ZERO(&a);
	FD_SET(0, &a);
	FD_SET(mid, &a);
	FD_SET(last, &a);
	/* A structure type: copy by assignment. */
	b = a;
	FD_ZERO(&a);
	ok = FD_ISSET(0, &b) && FD_ISSET(mid, &b) && FD_ISSET(last, &b);
	pc_check(ok, "a copy of an fd_set lost members");
	pc_check(!FD_ISSET(1, &b) && !FD_ISSET(last - 1, &b), "a copy of an fd_set gained members");
	pc_check(!FD_ISSET(0, &a) && !FD_ISSET(mid, &a) && !FD_ISSET(last, &a), "FD_ZERO of the original did not empty it");
	pc_check(FD_ISSET(0, &b) && FD_ISSET(last, &b), "emptying the original changed the copy");
	FD_CLR(mid, &b);
	pc_check(!FD_ISSET(mid, &b) && FD_ISSET(0, &b) && FD_ISSET(last, &b), "FD_CLR in the copy removed the wrong members");
	pc_pass();
#endif
}
