/*
 * <netdb.h> struct servent
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netdb.h.009
 *
 * Only <netdb.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <netdb.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct servent)];	/* a complete type */
static struct servent *ps;

int main(void)
{
	pc_begin("XBD.netdb.h.009");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->s_name, char * *))
		pc_fail("struct servent member s_name is not of type char *");
	if (!IS_T(&ps->s_aliases, char ** *))
		pc_fail("struct servent member s_aliases is not of type char **");
	if (!IS_T(&ps->s_port, int *))
		pc_fail("struct servent member s_port is not of type int");
	if (!IS_T(&ps->s_proto, char * *))
		pc_fail("struct servent member s_proto is not of type char *");
	pc_pass();
}
