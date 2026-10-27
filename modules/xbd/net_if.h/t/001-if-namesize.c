/*
 * IF_NAMESIZE is a positive integer constant expression usable as an
 *   array size.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.net_if.h.001
 */
#include <net/if.h>
#include "pc.h"


#ifdef IF_NAMESIZE
static char name[IF_NAMESIZE];
enum { size = IF_NAMESIZE };
#endif

int main(void)
{
	pc_begin("XBD.net_if.h.001");
	pc_test();
#ifndef IF_NAMESIZE
	pc_fail("<net/if.h> does not define IF_NAMESIZE");
#else
	pc_check(size > 0 && sizeof name == (size_t)size, "IF_NAMESIZE is %d", (int)size);
	pc_pass();
#endif
}
