/*
 * strtoimax: negative u.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtoimax.026
 */
#define IC_UMAX
#include "ic.h"
#include <string.h>

static const struct ic_case tab[] = {
	{ "-0", 10, 0, -1 },
};

int main(void)
{
	int j, err;
	char *e;
	R r;
	pc_begin("XSH.strtoimax.026");
	pc_test();
	ic_run(tab, 1);
	for (j = 0; j < IC_N; j++) {
		errno = EDOM;
		r = ic_fns[j].f("-1", &e, 10);
		err = errno;
		pc_check(r == ic_fns[j].max && err == EDOM, "%s(\"-1\") did not give the maximum with errno unchanged", ic_fns[j].name);
		errno = EDOM;
		r = ic_fns[j].f("-2", &e, 10);
		err = errno;
		pc_check(r == ic_fns[j].max - 1 && err == EDOM, "%s(\"-2\") did not give the maximum minus one with errno unchanged", ic_fns[j].name);
		errno = EDOM;
		r = ic_fns[j].f("-0x1", &e, 16);
		err = errno;
		pc_check(r == ic_fns[j].max && err == EDOM, "%s(\"-0x1\", 16) did not give the maximum with errno unchanged", ic_fns[j].name);
	}
	pc_pass();
}
