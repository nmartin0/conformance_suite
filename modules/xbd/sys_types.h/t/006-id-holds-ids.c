/*
 * id_t holds pid_t, uid_t and gid_t values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_types.h.006
 */
#include <sys/types.h>
#include "pc.h"

_Static_assert(sizeof(id_t) >= sizeof(pid_t) && sizeof(id_t) >= sizeof(uid_t) && sizeof(id_t) >= sizeof(gid_t), "id_t is narrower than pid_t, uid_t or gid_t");

int main(void)
{
	pc_begin("XBD.sys_types.h.006");
	pc_test();
	pid_t p[] = { 0, 1, 12345, (pid_t)-1 };
	uid_t u[] = { 0, 1, 1000, (uid_t)-1, (uid_t)~(uid_t)0 >> 1 };
	gid_t g[] = { 0, 1, 1000, (gid_t)-1, (gid_t)~(gid_t)0 >> 1 };
	size_t i;

	for (i = 0; i < sizeof p / sizeof p[0]; i++) {
		id_t x = (id_t)p[i];
		pc_check((pid_t)x == p[i], "pid_t value %ld does not survive id_t", (long)p[i]);
	}
	for (i = 0; i < sizeof u / sizeof u[0]; i++) {
		id_t x = (id_t)u[i];
		pc_check((uid_t)x == u[i], "uid_t value %lu does not survive id_t", (unsigned long)u[i]);
	}
	for (i = 0; i < sizeof g / sizeof g[0]; i++) {
		id_t x = (id_t)g[i];
		pc_check((gid_t)x == g[i], "gid_t value %lu does not survive id_t", (unsigned long)g[i]);
	}
	pc_pass();
}
