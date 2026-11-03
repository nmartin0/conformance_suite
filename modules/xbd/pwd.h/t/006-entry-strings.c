/*
 * pw_dir and pw_shell of a found entry are not null pointers.
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
	pc_check(p->pw_dir != NULL, "pw_dir is a null pointer");
	pc_check(p->pw_shell != NULL, "pw_shell is a null pointer");
	pc_pass();
}
