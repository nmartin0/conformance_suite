/*
 * dladdr() of the address of a data object names that object.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dladdr.003
 */
#include "dlx.h"

int main(void)
{
	void *h, *s;
	Dl_info_t info;

	pc_begin("XSH.dladdr.003");
	h = dlopen(NULL, RTLD_NOW);
	pc_check(h != NULL, "dlopen(NULL) failed");
	s = dlsym(h, "environ");
	pc_check(s != NULL, "dlsym() did not find environ");
	memset(&info, 0, sizeof info);
	pc_test();
	pc_check(dladdr(s, &info) != 0, "dladdr() returned 0 for the address of environ");
	pc_check(info.dli_fname != NULL && info.dli_fname[0] != '\0', "dli_fname is null or empty");
	pc_check(info.dli_sname != NULL, "dli_sname is null for the address of an exported data object");
	pc_check(info.dli_saddr == s, "dli_saddr is not the address of the data object");
	(void)dlclose(h);
	pc_pass();
}
