/*
 * <mqueue.h> macros open flags
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.mqueue.h.005
 *
 * Only <mqueue.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <mqueue.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.mqueue.h.005");
	pc_test();
#ifndef O_RDONLY
	pc_fail("<mqueue.h> does not define O_RDONLY");
#endif
#ifndef O_WRONLY
	pc_fail("<mqueue.h> does not define O_WRONLY");
#endif
#ifndef O_RDWR
	pc_fail("<mqueue.h> does not define O_RDWR");
#endif
#ifndef O_CREAT
	pc_fail("<mqueue.h> does not define O_CREAT");
#endif
#ifndef O_EXCL
	pc_fail("<mqueue.h> does not define O_EXCL");
#endif
#ifndef O_NONBLOCK
	pc_fail("<mqueue.h> does not define O_NONBLOCK");
#endif
	pc_pass();
}
