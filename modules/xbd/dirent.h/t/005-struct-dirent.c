/*
 * <dirent.h> struct dirent
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.dirent.h.005
 *
 * Only <dirent.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <dirent.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct dirent)];	/* a complete type */
static struct dirent *ps;

int main(void)
{
	pc_begin("XBD.dirent.h.005");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->d_ino, ino_t *))
		pc_fail("struct dirent member d_ino is not of type ino_t");
	if (!IS_T(&ps->d_name[0], char *))
		pc_fail("struct dirent member d_name is not an array of char");
	if (IS_T(&ps->d_name, char **))
		pc_fail("struct dirent member d_name is not an array");
	pc_pass();
}
