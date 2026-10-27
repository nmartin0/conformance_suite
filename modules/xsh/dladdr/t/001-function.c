/*
 * dladdr() of the address of a function fills the structure for that function.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dladdr.001
 */
#include "dlx.h"

int main(void)
{
	void *h, *s;
	Dl_info_t info;

	pc_begin("XSH.dladdr.001");
	h = dlopen(NULL, RTLD_NOW);
	pc_check(h != NULL, "dlopen(NULL) failed");
	s = dlsym(h, "getpid");
	pc_check(s != NULL, "dlsym() did not find getpid");
	memset(&info, 0, sizeof info);
	pc_test();
	pc_check(dladdr(s, &info) != 0, "dladdr() returned 0 for the address of getpid");
	pc_check(info.dli_fname != NULL && info.dli_fname[0] != '\0', "dli_fname is null or empty");
	pc_check(info.dli_fbase != NULL, "dli_fbase is null");
	pc_check((char *)info.dli_fbase <= (char *)s, "the base of the object is above the address");
	pc_check(info.dli_sname != NULL, "dli_sname is null for the address of an exported function");
	pc_check(info.dli_saddr == s, "dli_saddr is not the address of the function");
	(void)dlclose(h);
	pc_pass();
}
