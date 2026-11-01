/*
 * <pwd.h> types base
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pwd.h.002
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

typedef gid_t *pc_t0;
typedef uid_t *pc_t1;
typedef size_t *pc_t2;

int main(void)
{
	pc_begin("XBD.pwd.h.002");
	pc_test();
	pc_pass();
}
