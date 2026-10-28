/*
 * <fcntl.h> macros seek
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.019
 *
 * Only <fcntl.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <fcntl.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.fcntl.h.019");
	pc_test();
#ifndef SEEK_SET
	pc_fail("<fcntl.h> does not define SEEK_SET");
#endif
#ifndef SEEK_CUR
	pc_fail("<fcntl.h> does not define SEEK_CUR");
#endif
#ifndef SEEK_END
	pc_fail("<fcntl.h> does not define SEEK_END");
#endif
	pc_pass();
}
