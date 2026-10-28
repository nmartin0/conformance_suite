/*
 * <sys/shm.h> struct shmid_ds
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_shm.h.005
 *
 * Only <sys/shm.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/shm.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct shmid_ds)];	/* a complete type */
static struct shmid_ds *ps;

int main(void)
{
	pc_begin("XBD.sys_shm.h.005");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->shm_perm, struct ipc_perm *))
		pc_fail("struct shmid_ds member shm_perm is not of type struct ipc_perm");
	if (!IS_T(&ps->shm_segsz, size_t *))
		pc_fail("struct shmid_ds member shm_segsz is not of type size_t");
	if (!IS_T(&ps->shm_lpid, pid_t *))
		pc_fail("struct shmid_ds member shm_lpid is not of type pid_t");
	if (!IS_T(&ps->shm_cpid, pid_t *))
		pc_fail("struct shmid_ds member shm_cpid is not of type pid_t");
	if (!IS_T(&ps->shm_nattch, shmatt_t *))
		pc_fail("struct shmid_ds member shm_nattch is not of type shmatt_t");
	if (!IS_T(&ps->shm_atime, time_t *))
		pc_fail("struct shmid_ds member shm_atime is not of type time_t");
	if (!IS_T(&ps->shm_dtime, time_t *))
		pc_fail("struct shmid_ds member shm_dtime is not of type time_t");
	if (!IS_T(&ps->shm_ctime, time_t *))
		pc_fail("struct shmid_ds member shm_ctime is not of type time_t");
	pc_pass();
}
