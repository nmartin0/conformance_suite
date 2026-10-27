/*
 * P_ALL, P_PGID and P_PID can be stored in an idtype_t and are three
 * different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_wait.h.004
 */
#include "pc.h"
#include <sys/wait.h>

int main(void)
{
	idtype_t a = P_ALL, g = P_PGID, p = P_PID;

	pc_begin("XBD.sys_wait.h.004");
	pc_test();
	pc_check(a != g && a != p && g != p, "P_ALL, P_PGID and P_PID are not three different values");
	pc_pass();
}
