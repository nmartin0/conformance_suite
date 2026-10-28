/*
 * <unistd.h> variables
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.031
 *
 * Only <unistd.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <unistd.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.unistd.h.031");
	pc_test();
	if (!IS_T(&optarg, char * *))
		pc_fail("optarg is not declared as char *");
	if (!IS_T(&opterr, int *))
		pc_fail("opterr is not declared as int");
	if (!IS_T(&optind, int *))
		pc_fail("optind is not declared as int");
	if (!IS_T(&optopt, int *))
		pc_fail("optopt is not declared as int");
	pc_pass();
}
