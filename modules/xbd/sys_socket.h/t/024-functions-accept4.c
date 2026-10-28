/*
 * <sys/socket.h> functions accept4
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_socket.h.024
 *
 * Only <sys/socket.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/socket.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.sys_socket.h.024");
	pc_test();
	if (!IS_T(&(accept4), int (*)(int, struct sockaddr *restrict, socklen_t *restrict, int)))
		pc_fail("accept4() is not declared as int accept4(int, struct sockaddr *restrict, socklen_t *restrict, int)");
	if (IS_T(&(accept4), int (*)(int, struct sockaddr *restrict, socklen_t *restrict, int, int)))
		pc_fail("accept4() is declared without a prototype");
	pc_pass();
}
