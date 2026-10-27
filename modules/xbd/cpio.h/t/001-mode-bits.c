/*
 * The cpio permission and special mode bits have the octal values the
 *   page gives.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.cpio.h.001
 */
#include <cpio.h>
#include "pc.h"

static const struct { const char *name; unsigned long expect; unsigned long value; int missing; } v[] = {
#ifdef C_IRUSR
	{ "C_IRUSR", 0000400, (unsigned long)(C_IRUSR), 0 },
#else
	{ "C_IRUSR", 0000400, 0, 1 },
#endif
#ifdef C_IWUSR
	{ "C_IWUSR", 0000200, (unsigned long)(C_IWUSR), 0 },
#else
	{ "C_IWUSR", 0000200, 0, 1 },
#endif
#ifdef C_IXUSR
	{ "C_IXUSR", 0000100, (unsigned long)(C_IXUSR), 0 },
#else
	{ "C_IXUSR", 0000100, 0, 1 },
#endif
#ifdef C_IRGRP
	{ "C_IRGRP", 0000040, (unsigned long)(C_IRGRP), 0 },
#else
	{ "C_IRGRP", 0000040, 0, 1 },
#endif
#ifdef C_IWGRP
	{ "C_IWGRP", 0000020, (unsigned long)(C_IWGRP), 0 },
#else
	{ "C_IWGRP", 0000020, 0, 1 },
#endif
#ifdef C_IXGRP
	{ "C_IXGRP", 0000010, (unsigned long)(C_IXGRP), 0 },
#else
	{ "C_IXGRP", 0000010, 0, 1 },
#endif
#ifdef C_IROTH
	{ "C_IROTH", 0000004, (unsigned long)(C_IROTH), 0 },
#else
	{ "C_IROTH", 0000004, 0, 1 },
#endif
#ifdef C_IWOTH
	{ "C_IWOTH", 0000002, (unsigned long)(C_IWOTH), 0 },
#else
	{ "C_IWOTH", 0000002, 0, 1 },
#endif
#ifdef C_IXOTH
	{ "C_IXOTH", 0000001, (unsigned long)(C_IXOTH), 0 },
#else
	{ "C_IXOTH", 0000001, 0, 1 },
#endif
#ifdef C_ISUID
	{ "C_ISUID", 0004000, (unsigned long)(C_ISUID), 0 },
#else
	{ "C_ISUID", 0004000, 0, 1 },
#endif
#ifdef C_ISGID
	{ "C_ISGID", 0002000, (unsigned long)(C_ISGID), 0 },
#else
	{ "C_ISGID", 0002000, 0, 1 },
#endif
#ifdef C_ISVTX
	{ "C_ISVTX", 0001000, (unsigned long)(C_ISVTX), 0 },
#else
	{ "C_ISVTX", 0001000, 0, 1 },
#endif
};

int main(void)
{
	size_t i;

	pc_begin("XBD.cpio.h.001");
	pc_test();
	for (i = 0; i < sizeof v / sizeof v[0]; i++) {
		pc_check(!v[i].missing, "the header does not define %s", v[i].name);
		pc_check(v[i].value == v[i].expect, "%s is %#lo, not %#lo", v[i].name, v[i].value, v[i].expect);
	}
	pc_pass();
}
