/*
 * dladdr() of an address in no mapped object returns 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dladdr.002
 */
#include "dlx.h"

int main(void)
{
	Dl_info_t info;

	pc_begin("XSH.dladdr.002");
	memset(&info, 0, sizeof info);
	pc_test();
	pc_check(dladdr(NULL, &info) == 0, "dladdr() of the null address returned non-zero");
	pc_pass();
}
