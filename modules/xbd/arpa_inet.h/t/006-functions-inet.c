/*
 * <arpa/inet.h> functions inet
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.arpa_inet.h.006
 *
 * Only <arpa/inet.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <arpa/inet.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.arpa_inet.h.006");
	pc_test();
	if (!IS_T(&(inet_addr), in_addr_t (*)(const char *)))
		pc_fail("inet_addr() is not declared as in_addr_t inet_addr(const char *)");
	if (IS_T(&(inet_addr), in_addr_t (*)(const char *, int)))
		pc_fail("inet_addr() is declared without a prototype");
	if (!IS_T(&(inet_ntoa), char *(*)(struct in_addr)))
		pc_fail("inet_ntoa() is not declared as char *inet_ntoa(struct in_addr)");
	if (IS_T(&(inet_ntoa), char *(*)(struct in_addr, int)))
		pc_fail("inet_ntoa() is declared without a prototype");
	if (!IS_T(&(inet_ntop), const char *(*)(int, const void *restrict, char *restrict, socklen_t)))
		pc_fail("inet_ntop() is not declared as const char *inet_ntop(int, const void *restrict, char *restrict, socklen_t)");
	if (IS_T(&(inet_ntop), const char *(*)(int, const void *restrict, char *restrict, socklen_t, int)))
		pc_fail("inet_ntop() is declared without a prototype");
	if (!IS_T(&(inet_pton), int (*)(int, const char *restrict, void *restrict)))
		pc_fail("inet_pton() is not declared as int inet_pton(int, const char *restrict, void *restrict)");
	if (IS_T(&(inet_pton), int (*)(int, const char *restrict, void *restrict, int)))
		pc_fail("inet_pton() is declared without a prototype");
	pc_pass();
}
