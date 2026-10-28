/*
 * <netdb.h> struct protoent
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netdb.h.008
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

typedef char pc_c0[sizeof(struct protoent)];	/* a complete type */
static struct protoent *ps;

int main(void)
{
	pc_begin("XBD.netdb.h.008");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->p_name, char * *))
		pc_fail("struct protoent member p_name is not of type char *");
	if (!IS_T(&ps->p_aliases, char ** *))
		pc_fail("struct protoent member p_aliases is not of type char **");
	if (!IS_T(&ps->p_proto, int *))
		pc_fail("struct protoent member p_proto is not of type int");
	pc_pass();
}
