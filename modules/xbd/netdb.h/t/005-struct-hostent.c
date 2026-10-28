/*
 * <netdb.h> struct hostent
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netdb.h.005
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

typedef char pc_c0[sizeof(struct hostent)];	/* a complete type */
static struct hostent *ps;

int main(void)
{
	pc_begin("XBD.netdb.h.005");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->h_name, char * *))
		pc_fail("struct hostent member h_name is not of type char *");
	if (!IS_T(&ps->h_aliases, char ** *))
		pc_fail("struct hostent member h_aliases is not of type char **");
	if (!IS_T(&ps->h_addrtype, int *))
		pc_fail("struct hostent member h_addrtype is not of type int");
	if (!IS_T(&ps->h_length, int *))
		pc_fail("struct hostent member h_length is not of type int");
	if (!IS_T(&ps->h_addr_list, char ** *))
		pc_fail("struct hostent member h_addr_list is not of type char **");
	pc_pass();
}
