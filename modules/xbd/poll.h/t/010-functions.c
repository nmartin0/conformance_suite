/*
 * <poll.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.poll.h.010
 *
 * Only <poll.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <poll.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.poll.h.010");
	pc_test();
	if (!IS_T(&(poll), int (*)(struct pollfd [], nfds_t, int)))
		pc_fail("poll() is not declared as int poll(struct pollfd [], nfds_t, int)");
	if (IS_T(&(poll), int (*)(struct pollfd [], nfds_t, int, int)))
		pc_fail("poll() is declared without a prototype");
	pc_pass();
}
