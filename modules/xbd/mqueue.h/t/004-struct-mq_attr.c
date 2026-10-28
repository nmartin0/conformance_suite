/*
 * <mqueue.h> struct mq_attr
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.mqueue.h.004
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

typedef char pc_c0[sizeof(struct mq_attr)];	/* a complete type */
static struct mq_attr *ps;

int main(void)
{
	pc_begin("XBD.mqueue.h.004");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->mq_flags, long *))
		pc_fail("struct mq_attr member mq_flags is not of type long");
	if (!IS_T(&ps->mq_maxmsg, long *))
		pc_fail("struct mq_attr member mq_maxmsg is not of type long");
	if (!IS_T(&ps->mq_msgsize, long *))
		pc_fail("struct mq_attr member mq_msgsize is not of type long");
	if (!IS_T(&ps->mq_curmsgs, long *))
		pc_fail("struct mq_attr member mq_curmsgs is not of type long");
	pc_pass();
}
