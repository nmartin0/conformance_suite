/*
 * fpathconf() of the terminal variables on the controlling terminal.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fpathconf.007
 * MUTANT    fpathconf.max_canon_small
 * MUTANT    fpathconf.tty_einval
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SENTINEL 12345

int main(void)
{
	static const struct {
		const char *name;
		int pc;
		long min;
	} vars[] = { { "_PC_MAX_CANON", _PC_MAX_CANON, 255 }, { "_PC_MAX_INPUT", _PC_MAX_INPUT, 255 },
		     { "_PC_VDISABLE", _PC_VDISABLE, -1 } };
	char name[L_ctermid];
	int t, i, e;
	long r;

	pc_begin("XSH.fpathconf.007");
	if (ctermid(name) == NULL || name[0] == '\0')
		pc_unresolved("ctermid() gives no terminal name");
	t = open(name, O_RDWR | O_NOCTTY);
	if (t < 0)
		pc_unresolved("no controlling terminal: open(\"%s\") failed with %s", name, pc_errname(errno));
	if (isatty(t) != 1)
		pc_unresolved("%s is not a terminal", name);
	pc_test();
	for (i = 0; i < 3; i++) {
		errno = SENTINEL;
		r = fpathconf(t, vars[i].pc);
		e = errno;
		if (r == -1) {
			pc_check(e == SENTINEL, "fpathconf() of %s on a terminal failed with %s", vars[i].name,
				 pc_errname(e));
			continue;
		}
		pc_check(r >= vars[i].min, "fpathconf() of %s on a terminal returned %ld, below %ld", vars[i].name, r,
			 vars[i].min);
	}
	pc_pass();
}
