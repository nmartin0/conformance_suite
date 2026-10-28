/*
 * <net/if.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.net_if.h.003
 *
 * Only <net/if.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <net/if.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.net_if.h.003");
	pc_test();
	if (!IS_T(&(if_freenameindex), void (*)(struct if_nameindex *)))
		pc_fail("if_freenameindex() is not declared as void if_freenameindex(struct if_nameindex *)");
	if (IS_T(&(if_freenameindex), void (*)(struct if_nameindex *, int)))
		pc_fail("if_freenameindex() is declared without a prototype");
	if (!IS_T(&(if_indextoname), char *(*)(unsigned, char *)))
		pc_fail("if_indextoname() is not declared as char *if_indextoname(unsigned, char *)");
	if (IS_T(&(if_indextoname), char *(*)(unsigned, char *, int)))
		pc_fail("if_indextoname() is declared without a prototype");
	if (!IS_T(&(if_nameindex), struct if_nameindex *(*)(void)))
		pc_fail("if_nameindex() is not declared as struct if_nameindex *if_nameindex(void)");
	if (IS_T(&(if_nameindex), struct if_nameindex *(*)(int)))
		pc_fail("if_nameindex() is declared without a prototype");
	if (!IS_T(&(if_nametoindex), unsigned (*)(const char *)))
		pc_fail("if_nametoindex() is not declared as unsigned if_nametoindex(const char *)");
	if (IS_T(&(if_nametoindex), unsigned (*)(const char *, int)))
		pc_fail("if_nametoindex() is declared without a prototype");
	pc_pass();
}
