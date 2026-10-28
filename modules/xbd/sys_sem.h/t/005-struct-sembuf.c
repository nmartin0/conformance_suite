/*
 * <sys/sem.h> struct sembuf
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_sem.h.005
 *
 * Only <sys/sem.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/sem.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct sembuf)];	/* a complete type */
static struct sembuf *ps;

int main(void)
{
	pc_begin("XBD.sys_sem.h.005");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->sem_num, unsigned short *))
		pc_fail("struct sembuf member sem_num is not of type unsigned short");
	if (!IS_T(&ps->sem_op, short *))
		pc_fail("struct sembuf member sem_op is not of type short");
	if (!IS_T(&ps->sem_flg, short *))
		pc_fail("struct sembuf member sem_flg is not of type short");
	pc_pass();
}
