/*
 * FNM_IGNORECASE acts as FNM_CASEFOLD: matching ignores case with either,
 * alone or with FNM_PATHNAME.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fnmatch.h.002
 */
#include "pc.h"
#include <fnmatch.h>

int main(void)
{
	pc_begin("XBD.fnmatch.h.002");
#if !defined(FNM_CASEFOLD) || !defined(FNM_IGNORECASE) || !defined(FNM_PATHNAME)
	pc_unresolved("FNM_CASEFOLD, FNM_IGNORECASE or FNM_PATHNAME is missing (the header check reports it)");
#else
	pc_test();
	pc_check(fnmatch("abc", "ABC", FNM_CASEFOLD) == 0, "fnmatch(abc, ABC, FNM_CASEFOLD) does not match");
	pc_check(fnmatch("abc", "ABC", FNM_IGNORECASE) == 0, "fnmatch(abc, ABC, FNM_IGNORECASE) does not match");
	pc_check(fnmatch("abc", "ABC", 0) != 0, "fnmatch(abc, ABC, 0) matches");
	pc_check(fnmatch("a/B", "A/b", FNM_CASEFOLD | FNM_PATHNAME) == 0, "FNM_CASEFOLD | FNM_PATHNAME does not match a/B with A/b");
	pc_check(fnmatch("a/B", "A/b", FNM_IGNORECASE | FNM_PATHNAME) == 0, "FNM_IGNORECASE | FNM_PATHNAME does not match a/B with A/b");
	pc_check(fnmatch("a?", "A/", FNM_IGNORECASE | FNM_PATHNAME) == fnmatch("a?", "A/", FNM_CASEFOLD | FNM_PATHNAME), "FNM_IGNORECASE and FNM_CASEFOLD give different results with FNM_PATHNAME");
	pc_pass();
#endif
}
