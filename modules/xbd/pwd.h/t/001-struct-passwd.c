/*
 * <pwd.h> struct passwd
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pwd.h.001
 *
 * Only <pwd.h> is included (not pc.h, which brings in other headers), so a
 * name the header fails to provide stops the build, which the module
 * reports as FAIL.
 */
#include <pwd.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct passwd)];	/* a complete type */
static struct passwd *ps;

int main(void)
{
	pc_begin("XBD.pwd.h.001");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->pw_name, char * *))
		pc_fail("struct passwd member pw_name is not of type char *");
	if (!IS_T(&ps->pw_uid, uid_t *))
		pc_fail("struct passwd member pw_uid is not of type uid_t");
	if (!IS_T(&ps->pw_gid, gid_t *))
		pc_fail("struct passwd member pw_gid is not of type gid_t");
	if (!IS_T(&ps->pw_dir, char * *))
		pc_fail("struct passwd member pw_dir is not of type char *");
	if (!IS_T(&ps->pw_shell, char * *))
		pc_fail("struct passwd member pw_shell is not of type char *");
	pc_pass();
}
