/*
 * <utmpx.h> struct utmpx
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.utmpx.h.002
 *
 * Only <utmpx.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <utmpx.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct utmpx)];	/* a complete type */
static struct utmpx *ps;

int main(void)
{
	pc_begin("XBD.utmpx.h.002");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->ut_user[0], char *))
		pc_fail("struct utmpx member ut_user is not an array of char");
	if (IS_T(&ps->ut_user, char **))
		pc_fail("struct utmpx member ut_user is not an array");
	if (!IS_T(&ps->ut_id[0], char *))
		pc_fail("struct utmpx member ut_id is not an array of char");
	if (IS_T(&ps->ut_id, char **))
		pc_fail("struct utmpx member ut_id is not an array");
	if (!IS_T(&ps->ut_line[0], char *))
		pc_fail("struct utmpx member ut_line is not an array of char");
	if (IS_T(&ps->ut_line, char **))
		pc_fail("struct utmpx member ut_line is not an array");
	if (!IS_T(&ps->ut_pid, pid_t *))
		pc_fail("struct utmpx member ut_pid is not of type pid_t");
	if (!IS_T(&ps->ut_type, short *))
		pc_fail("struct utmpx member ut_type is not of type short");
	if (!IS_T(&ps->ut_tv, struct timeval *))
		pc_fail("struct utmpx member ut_tv is not of type struct timeval");
	pc_pass();
}
