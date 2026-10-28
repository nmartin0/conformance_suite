/*
 * <netinet/in.h> struct in6_addr
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netinet_in.h.012
 *
 * Only <netinet/in.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <netinet/in.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef char pc_c0[sizeof(struct in6_addr)];	/* a complete type */
static struct in6_addr *ps;

int main(void)
{
	pc_begin("XBD.netinet_in.h.012");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->s6_addr[0], uint8_t *))
		pc_fail("struct in6_addr member s6_addr is not an array of uint8_t");
	if (!IS_T(&ps->s6_addr, uint8_t (*)[16]))
		pc_fail("struct in6_addr member s6_addr is not an array");
	pc_pass();
}
