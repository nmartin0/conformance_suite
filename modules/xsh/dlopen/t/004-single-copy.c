/*
 * Opening the object that defines getpid() twice by its pathname gives access to one copy: the symbols found are usable and equal.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dlopen.004
 */
#define DLX_OBJECT
#include "dlx.h"

int main(void)
{
	char path[4096];
	void *h1, *h2, *s1, *s2;

	pc_begin("XSH.dlopen.004");
	dlx_object(path, sizeof path);
	pc_test();
	h1 = dlopen(path, RTLD_NOW | RTLD_LOCAL);
	if (h1 == NULL)
		pc_unresolved("dlopen of %s failed: the kind of object is not eligible?", path);
	h2 = dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
	pc_check(h2 != NULL, "the second dlopen of %s failed", path);
	s1 = dlsym(h1, "getpid");
	s2 = dlsym(h2, "getpid");
	pc_check(s1 != NULL && s2 != NULL, "getpid not found through the handles");
	pc_check(dlx_getpid(s1) == getpid(), "getpid through the first handle gave a wrong result");
	pc_check(dlx_getpid(s2) == getpid(), "getpid through the second handle gave a wrong result");
	pc_check(s1 == s2, "the two handles give different addresses for getpid: more than one copy is loaded");
	(void)dlclose(h2);
	(void)dlclose(h1);
	pc_pass();
}
