/*
 * rlim_t is an unsigned integer type, and RLIM_INFINITY, RLIM_SAVED_MAX
 * and RLIM_SAVED_CUR are usable in #if and are values of rlim_t.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_resource.h.004
 */
#include "pc.h"
#include <sys/resource.h>

#if defined(RLIM_INFINITY) && defined(RLIM_SAVED_MAX) && defined(RLIM_SAVED_CUR)
#if !((RLIM_INFINITY) || 1)
#error "RLIM_INFINITY is not usable in #if"
#endif
#if !((RLIM_SAVED_MAX) || 1)
#error "RLIM_SAVED_MAX is not usable in #if"
#endif
#if !((RLIM_SAVED_CUR) || 1)
#error "RLIM_SAVED_CUR is not usable in #if"
#endif
#endif

int main(void)
{
	pc_begin("XBD.sys_resource.h.004");
#if !defined(RLIM_INFINITY) || !defined(RLIM_SAVED_MAX) || !defined(RLIM_SAVED_CUR)
	pc_unresolved("an RLIM_ constant is missing (the header check reports it)");
#else
	pc_test();
	pc_check((rlim_t)-1 > 0, "rlim_t is not an unsigned type");
	pc_check((rlim_t)0.5 == 0, "rlim_t is not an integer type");
	pc_check((unsigned long long)(rlim_t)(RLIM_INFINITY) == (unsigned long long)(RLIM_INFINITY), "RLIM_INFINITY is not a value of rlim_t");
	pc_check((unsigned long long)(rlim_t)(RLIM_SAVED_MAX) == (unsigned long long)(RLIM_SAVED_MAX), "RLIM_SAVED_MAX is not a value of rlim_t");
	pc_check((unsigned long long)(rlim_t)(RLIM_SAVED_CUR) == (unsigned long long)(RLIM_SAVED_CUR), "RLIM_SAVED_CUR is not a value of rlim_t");
	pc_pass();
#endif
}
