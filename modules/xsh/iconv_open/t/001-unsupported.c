/*
 * A conversion that names a code set nobody supports fails with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iconv_open.001
 */
#include "iv.h"

static void expect_fail(const char *to, const char *from)
{
	iconv_t cd;

	errno = 0;
	cd = iconv_open(to, from);
	if (cd != IV_BAD) {
		(void)iconv_close(cd);
		pc_fail("iconv_open(\"%s\", \"%s\") returned a descriptor", to, from);
	}
	pc_check(errno == EINVAL, "iconv_open(\"%s\", \"%s\") set errno to %s, expected EINVAL",
	    to, from, pc_errname(errno));
}

int main(void)
{
	pc_begin("XSH.iconv_open.001");
	pc_test();
	expect_fail("UTF-8", "pc-no-such-code-set-q7x");
	expect_fail("pc-no-such-code-set-q7x", "UTF-8");
	expect_fail("pc-no-such-code-set-q7x", "pc-other-missing-set-z3");
	pc_pass();
}
