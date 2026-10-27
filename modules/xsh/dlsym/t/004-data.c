/*
 * dlsym() of a data object gives the address of the object.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dlsym.004
 */
#include "dlx.h"

extern char **environ;

int main(void)
{
	void *h, *s;
	char **v;

	pc_begin("XSH.dlsym.004");
	h = dlopen(NULL, RTLD_NOW);
	pc_check(h != NULL, "dlopen(NULL) failed");
	pc_check(environ != NULL, "environ is null");
	pc_test();
	s = dlsym(h, "environ");
	if (s == NULL)
		pc_unresolved("environ is not among the symbols the program image exports (the page leaves the set open)");
	memcpy(&v, s, sizeof v);
	pc_check(v == environ, "the object at the address is not environ");
	(void)dlclose(h);
	pc_pass();
}
