/*
 * Two handles of one object can each be closed with result 0, and the second stays usable after the first is closed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dlclose.002
 */
#define DLX_OBJECT
#include "dlx.h"

int main(void)
{
	char path[4096];
	void *h1, *h2, *s;

	pc_begin("XSH.dlclose.002");
	dlx_object(path, sizeof path);
	h1 = dlopen(path, RTLD_NOW);
	if (h1 == NULL)
		pc_unresolved("dlopen of %s failed: the kind of object is not eligible?", path);
	h2 = dlopen(path, RTLD_NOW);
	pc_check(h2 != NULL, "the second dlopen of %s failed", path);
	pc_test();
	pc_check(dlclose(h1) == 0, "dlclose() of the first handle did not return 0");
	s = dlsym(h2, "getpid");
	pc_check(s != NULL, "getpid not found through the second handle after the first was closed");
	pc_check(dlx_getpid(s) == getpid(), "getpid through the second handle gave a wrong result");
	pc_check(dlclose(h2) == 0, "dlclose() of the second handle did not return 0");
	pc_pass();
}
