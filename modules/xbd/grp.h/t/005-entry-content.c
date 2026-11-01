/*
 * A group entry found through the header's functions has a name, the
 * group ID asked for and an array of member names ended by a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.grp.h.005
 */
#include "pc.h"
#include <grp.h>
#include <unistd.h>

#define MAXMEM 1000000L		/* bound on the walk: an unterminated array is a failure */

int main(void)
{
	gid_t want[2];
	struct group *g = NULL;
	int i;
	long k;

	pc_begin("XBD.grp.h.005");
	want[0] = getgid();
	want[1] = getegid();
	for (i = 0; i < 2 && g == NULL; i++)
		g = getgrgid(want[i]);
	if (g == NULL)
		pc_unresolved("neither the real nor the effective group ID of this process has a group entry");
	pc_test();
	pc_check(g->gr_gid == want[i - 1], "gr_gid is %ld, the ID asked for is %ld",
	    (long)g->gr_gid, (long)want[i - 1]);
	pc_check(g->gr_name != NULL, "gr_name is a null pointer");
	pc_check(g->gr_mem != NULL, "gr_mem is a null pointer, not an array ended by one");
	for (k = 0; k < MAXMEM && g->gr_mem[k] != NULL; k++)
		;
	pc_check(k < MAXMEM, "gr_mem has no null pointer within %ld elements", MAXMEM);
	pc_pass();
}
