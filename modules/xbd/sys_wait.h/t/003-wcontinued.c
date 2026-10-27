/*
 * On XSI systems WCONTINUED is nonzero and shares no bits with WNOHANG,
 * WUNTRACED, WEXITED, WNOWAIT or WSTOPPED.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_wait.h.003
 */
#include "pc.h"
#include <sys/wait.h>

int main(void)
{
	pc_begin("XBD.sys_wait.h.003");
#ifndef WCONTINUED
	pc_unresolved("WCONTINUED is missing (the header check reports it)");
#else
	unsigned long long w = (unsigned long long)(WCONTINUED);

	pc_test();
	pc_check(w != 0, "WCONTINUED is zero");
#ifdef WNOHANG
	pc_check((w & (unsigned long long)(WNOHANG)) == 0, "WCONTINUED and WNOHANG have bits in common");
#endif
#ifdef WUNTRACED
	pc_check((w & (unsigned long long)(WUNTRACED)) == 0, "WCONTINUED and WUNTRACED have bits in common");
#endif
#ifdef WEXITED
	pc_check((w & (unsigned long long)(WEXITED)) == 0, "WCONTINUED and WEXITED have bits in common");
#endif
#ifdef WNOWAIT
	pc_check((w & (unsigned long long)(WNOWAIT)) == 0, "WCONTINUED and WNOWAIT have bits in common");
#endif
#ifdef WSTOPPED
	pc_check((w & (unsigned long long)(WSTOPPED)) == 0, "WCONTINUED and WSTOPPED have bits in common");
#endif
	pc_pass();
#endif
}
