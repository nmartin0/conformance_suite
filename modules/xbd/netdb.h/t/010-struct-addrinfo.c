/*
 * <netdb.h> struct addrinfo
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netdb.h.010
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

typedef char pc_c0[sizeof(struct addrinfo)];	/* a complete type */
static struct addrinfo *ps;

int main(void)
{
	pc_begin("XBD.netdb.h.010");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->ai_flags, int *))
		pc_fail("struct addrinfo member ai_flags is not of type int");
	if (!IS_T(&ps->ai_family, int *))
		pc_fail("struct addrinfo member ai_family is not of type int");
	if (!IS_T(&ps->ai_socktype, int *))
		pc_fail("struct addrinfo member ai_socktype is not of type int");
	if (!IS_T(&ps->ai_protocol, int *))
		pc_fail("struct addrinfo member ai_protocol is not of type int");
	if (!IS_T(&ps->ai_addrlen, socklen_t *))
		pc_fail("struct addrinfo member ai_addrlen is not of type socklen_t");
	if (!IS_T(&ps->ai_addr, struct sockaddr * *))
		pc_fail("struct addrinfo member ai_addr is not of type struct sockaddr *");
	if (!IS_T(&ps->ai_canonname, char * *))
		pc_fail("struct addrinfo member ai_canonname is not of type char *");
	if (!IS_T(&ps->ai_next, struct addrinfo * *))
		pc_fail("struct addrinfo member ai_next is not of type struct addrinfo *");
	pc_pass();
}
