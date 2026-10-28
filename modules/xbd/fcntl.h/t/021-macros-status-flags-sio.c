/*
 * <fcntl.h> macros status flags sio
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.021
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
	pc_begin("XBD.fcntl.h.021");
	pc_test();
#ifndef O_DSYNC
	pc_fail("<fcntl.h> does not define O_DSYNC");
#elif !(O_DSYNC == O_DSYNC)
	pc_fail("O_DSYNC is not usable in #if");
#endif
#ifndef O_RSYNC
	pc_fail("<fcntl.h> does not define O_RSYNC");
#elif !(O_RSYNC == O_RSYNC)
	pc_fail("O_RSYNC is not usable in #if");
#endif
	pc_pass();
}
