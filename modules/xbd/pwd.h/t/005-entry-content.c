/*
 * A password entry found for this process's user ID has that ID in
 * pw_uid and a login name.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pwd.h.005
 */
#include "pc.h"
#include <pwd.h>
#include <unistd.h>

int main(void)
{
	uid_t u;
	struct passwd *p;

	pc_begin("XBD.pwd.h.005");
	u = getuid();
	p = getpwuid(u);
	if (p == NULL)
		pc_unresolved("the user ID of this process (%ld) has no password entry", (long)u);
	pc_test();
	pc_check(p->pw_uid == u, "pw_uid is %ld, the ID asked for is %ld", (long)p->pw_uid, (long)u);
	pc_check(p->pw_name != NULL, "pw_name is a null pointer");
	pc_pass();
}
