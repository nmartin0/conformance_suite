/*
 * dladdr() of an address inside a function reports the object and a symbol at or below the address.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dladdr.004
 */
#include "dlx.h"

int main(void)
{
	void *h, *s;
	const char *inner;
	Dl_info_t a, b;

	pc_begin("XSH.dladdr.004");
	h = dlopen(NULL, RTLD_NOW);
	pc_check(h != NULL, "dlopen(NULL) failed");
	s = dlsym(h, "getpid");
	pc_check(s != NULL, "dlsym() did not find getpid");
	memset(&a, 0, sizeof a);
	pc_check(dladdr(s, &a) != 0, "dladdr() returned 0 for the address of getpid");
	inner = (const char *)s + 1;
	memset(&b, 0, sizeof b);
	pc_test();
	pc_check(dladdr(inner, &b) != 0, "dladdr() returned 0 for an address inside getpid");
	pc_check(b.dli_fname != NULL && strcmp(b.dli_fname, a.dli_fname) == 0, "another object pathname");
	pc_check(b.dli_fbase == a.dli_fbase, "another object base");
	pc_check(b.dli_sname != NULL && b.dli_saddr != NULL, "no symbol found at or below the address");
	pc_check((const char *)b.dli_saddr <= inner, "the symbol address is above the address asked");
	(void)dlclose(h);
	pc_pass();
}
