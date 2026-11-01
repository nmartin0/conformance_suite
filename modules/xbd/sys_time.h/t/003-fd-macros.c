/*
 * <sys/time.h> alone provides FD_ZERO, FD_SET, FD_CLR, FD_ISSET and
 * FD_SETSIZE, and they work as a set of descriptors should.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_time.h.003
 *
 * Only <sys/time.h> is included (not pc.h, which brings in other headers),
 * so a name the header fails to provide stops the build, which the module
 * reports as FAIL.
 */
#include <sys/time.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

#if !((FD_SETSIZE) > 0)
#error "FD_SETSIZE is not usable in #if as a positive value"
#endif

int main(void)
{
	fd_set s;
	int pick[3], i, j;

	pc_begin("XBD.sys_time.h.003");
	pc_test();
	pick[0] = 0;
	pick[1] = (FD_SETSIZE) / 2;
	pick[2] = (FD_SETSIZE) - 1;

	FD_ZERO(&s);
	for (i = 0; i < 3; i++)
		if (FD_ISSET(pick[i], &s))
			pc_fail("descriptor %d is in the set after FD_ZERO", pick[i]);
	for (i = 0; i < 3; i++) {
		FD_SET(pick[i], &s);
		for (j = 0; j < 3; j++)
			if ((j <= i) != (FD_ISSET(pick[j], &s) != 0))
				pc_fail("after FD_SET of descriptor %d, membership of %d is wrong", pick[i], pick[j]);
	}
	FD_CLR(pick[1], &s);
	if (FD_ISSET(pick[1], &s))
		pc_fail("descriptor %d is still in the set after FD_CLR", pick[1]);
	if (!FD_ISSET(pick[0], &s) || !FD_ISSET(pick[2], &s))
		pc_fail("FD_CLR removed a descriptor other than the one given");
	FD_ZERO(&s);
	for (i = 0; i < 3; i++)
		if (FD_ISSET(pick[i], &s))
			pc_fail("descriptor %d is in the set after the second FD_ZERO", pick[i]);
	pc_pass();
}
