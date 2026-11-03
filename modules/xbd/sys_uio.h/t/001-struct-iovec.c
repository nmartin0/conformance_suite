/*
 * <sys/uio.h> struct iovec
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_uio.h.001
 *
 * Only <sys/uio.h> is included (not pc.h, which brings in other headers), so a
 * name the header fails to provide stops the build, which the module
 * reports as FAIL.
 */
#include <sys/uio.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct iovec)];	/* a complete type */
static struct iovec *ps;

int main(void)
{
	pc_begin("XBD.sys_uio.h.001");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->iov_base, void * *))
		pc_fail("struct iovec member iov_base is not of type void *");
	if (!IS_T(&ps->iov_len, size_t *))
		pc_fail("struct iovec member iov_len is not of type size_t");
	pc_pass();
}
