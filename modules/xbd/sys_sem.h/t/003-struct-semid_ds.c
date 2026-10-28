/*
 * <sys/sem.h> struct semid_ds
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_sem.h.003
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

typedef char pc_c0[sizeof(struct semid_ds)];	/* a complete type */
static struct semid_ds *ps;

int main(void)
{
	pc_begin("XBD.sys_sem.h.003");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->sem_perm, struct ipc_perm *))
		pc_fail("struct semid_ds member sem_perm is not of type struct ipc_perm");
	if (!IS_T(&ps->sem_nsems, unsigned short *))
		pc_fail("struct semid_ds member sem_nsems is not of type unsigned short");
	if (!IS_T(&ps->sem_otime, time_t *))
		pc_fail("struct semid_ds member sem_otime is not of type time_t");
	if (!IS_T(&ps->sem_ctime, time_t *))
		pc_fail("struct semid_ds member sem_ctime is not of type time_t");
	pc_pass();
}
