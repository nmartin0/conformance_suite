/*
 * The sporadic-server minimum value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.003
 */
#include <limits.h>
#include "pc.h"
#if !defined(_POSIX_SS_REPL_MAX) || (_POSIX_SS_REPL_MAX) != 4
#error "_POSIX_SS_REPL_MAX is missing or not the required value"
#endif

int main(void)
{
	pc_begin("XBD.limits.h.003");
	pc_test();
	pc_pass();
}
