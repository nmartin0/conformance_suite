/*
 * pw_dir and pw_shell of a found entry: a null pointer is not judged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pwd.h.006
 */
#include "pc.h"
#include <pwd.h>
#include <unistd.h>

int main(void)
{
	uid_t u;
	struct passwd *p;

	pc_begin("XBD.pwd.h.006");
	u = getuid();
	p = getpwuid(u);
	if (p == NULL)
		pc_unresolved("the user ID of this process (%ld) has no password entry", (long)u);
	pc_test();
	/* The page gives the members' types and meanings but does not say that
	 * a member always points to a string, so a null pointer is not a fault. */
	if (p->pw_dir == NULL)
		pc_unresolved("pw_dir is a null pointer; the page does not say whether it may be");
	if (p->pw_shell == NULL)
		pc_unresolved("pw_shell is a null pointer; the page does not say whether it may be");
	pc_pass();
}
