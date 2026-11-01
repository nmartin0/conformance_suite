/*
 * <grp.h> struct group
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.grp.h.001
 *
 * Only <grp.h> is included (not pc.h, which brings in other headers), so a
 * name the header fails to provide stops the build, which the module
 * reports as FAIL.
 */
#include <grp.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct group)];	/* a complete type */
static struct group *ps;

int main(void)
{
	pc_begin("XBD.grp.h.001");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->gr_name, char * *))
		pc_fail("struct group member gr_name is not of type char *");
	if (!IS_T(&ps->gr_gid, gid_t *))
		pc_fail("struct group member gr_gid is not of type gid_t");
	if (!IS_T(&ps->gr_mem, char ** *))
		pc_fail("struct group member gr_mem is not of type char **");
	pc_pass();
}
