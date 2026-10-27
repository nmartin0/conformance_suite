/*
 * TMAGIC, TMAGLEN, TVERSION and TVERSLEN have the values the page gives.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.tar.h.001
 */
#include <tar.h>
#include <string.h>
#include "pc.h"

int main(void)
{
	pc_begin("XBD.tar.h.001");
	pc_test();
#if !defined(TMAGIC) || !defined(TMAGLEN) || !defined(TVERSION) || !defined(TVERSLEN)
	pc_fail("<tar.h> does not define TMAGIC, TMAGLEN, TVERSION and TVERSLEN");
#else
	pc_check(strcmp(TMAGIC, "ustar") == 0, "TMAGIC is \"%s\", not \"ustar\"", TMAGIC);
	pc_check(TMAGLEN == 6, "TMAGLEN is %d, not 6", (int)TMAGLEN);
	pc_check(strlen(TMAGIC) + 1 == (size_t)TMAGLEN, "TMAGLEN is not the length of TMAGIC with its null byte");
	pc_check(strcmp(TVERSION, "00") == 0, "TVERSION is \"%s\", not \"00\"", TVERSION);
	pc_check(TVERSLEN == 2, "TVERSLEN is %d, not 2", (int)TVERSLEN);
	pc_check(strlen(TVERSION) == (size_t)TVERSLEN, "TVERSLEN is not the length of TVERSION");
	pc_pass();
#endif
}
