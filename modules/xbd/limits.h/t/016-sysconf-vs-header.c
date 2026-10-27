/*
 * Run-time limit values against the header values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.016
 */
#include <unistd.h>
#include <limits.h>
#include "pc.h"

int main(void)
{
	long r;

	pc_begin("XBD.limits.h.016");
	pc_test();
#ifdef NSIG_MAX
	r = sysconf(_SC_NSIG);
	pc_check(r == -1 || r <= (long)NSIG_MAX, "sysconf(_SC_NSIG) is %ld, above NSIG_MAX (%ld)", r, (long)NSIG_MAX);
#endif
#ifdef BC_BASE_MAX
	r = sysconf(_SC_BC_BASE_MAX);
	pc_check(r == -1 || r >= (long)BC_BASE_MAX, "sysconf(_SC_BC_BASE_MAX) is %ld, below BC_BASE_MAX (%ld)", r, (long)BC_BASE_MAX);
#endif
#ifdef BC_DIM_MAX
	r = sysconf(_SC_BC_DIM_MAX);
	pc_check(r == -1 || r >= (long)BC_DIM_MAX, "sysconf(_SC_BC_DIM_MAX) is %ld, below BC_DIM_MAX (%ld)", r, (long)BC_DIM_MAX);
#endif
#ifdef BC_SCALE_MAX
	r = sysconf(_SC_BC_SCALE_MAX);
	pc_check(r == -1 || r >= (long)BC_SCALE_MAX, "sysconf(_SC_BC_SCALE_MAX) is %ld, below BC_SCALE_MAX (%ld)", r, (long)BC_SCALE_MAX);
#endif
#ifdef BC_STRING_MAX
	r = sysconf(_SC_BC_STRING_MAX);
	pc_check(r == -1 || r >= (long)BC_STRING_MAX, "sysconf(_SC_BC_STRING_MAX) is %ld, below BC_STRING_MAX (%ld)", r, (long)BC_STRING_MAX);
#endif
#ifdef COLL_WEIGHTS_MAX
	r = sysconf(_SC_COLL_WEIGHTS_MAX);
	pc_check(r == -1 || r >= (long)COLL_WEIGHTS_MAX, "sysconf(_SC_COLL_WEIGHTS_MAX) is %ld, below COLL_WEIGHTS_MAX (%ld)", r, (long)COLL_WEIGHTS_MAX);
#endif
#ifdef EXPR_NEST_MAX
	r = sysconf(_SC_EXPR_NEST_MAX);
	pc_check(r == -1 || r >= (long)EXPR_NEST_MAX, "sysconf(_SC_EXPR_NEST_MAX) is %ld, below EXPR_NEST_MAX (%ld)", r, (long)EXPR_NEST_MAX);
#endif
#ifdef LINE_MAX
	r = sysconf(_SC_LINE_MAX);
	pc_check(r == -1 || r >= (long)LINE_MAX, "sysconf(_SC_LINE_MAX) is %ld, below LINE_MAX (%ld)", r, (long)LINE_MAX);
#endif
#ifdef NGROUPS_MAX
	r = sysconf(_SC_NGROUPS_MAX);
	pc_check(r == -1 || r >= (long)NGROUPS_MAX, "sysconf(_SC_NGROUPS_MAX) is %ld, below NGROUPS_MAX (%ld)", r, (long)NGROUPS_MAX);
#endif
#ifdef RE_DUP_MAX
	r = sysconf(_SC_RE_DUP_MAX);
	pc_check(r == -1 || r >= (long)RE_DUP_MAX, "sysconf(_SC_RE_DUP_MAX) is %ld, below RE_DUP_MAX (%ld)", r, (long)RE_DUP_MAX);
#endif
	pc_pass();
}
