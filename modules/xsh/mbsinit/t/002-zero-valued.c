/*
 * A zero-valued mbstate_t object describes an initial conversion state.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbsinit.002
 */
#include "pc.h"
#include <string.h>
#include <wchar.h>

static mbstate_t stat_st;

int main(void)
{
	mbstate_t st;
	int r1, r2;

	pc_begin("XSH.mbsinit.002");
	memset(&st, 0xA5, sizeof st);
	memset(&st, 0, sizeof st);
	pc_test();
	r1 = mbsinit(&st);
	r2 = mbsinit(&stat_st);
	pc_check(r1 != 0, "mbsinit() of an object set to all zero bytes returned 0");
	pc_check(r2 != 0, "mbsinit() of an object with static storage returned 0");
	pc_pass();
}
