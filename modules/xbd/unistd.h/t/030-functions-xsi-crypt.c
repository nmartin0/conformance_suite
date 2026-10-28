/*
 * <unistd.h> functions xsi crypt
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.030
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
	pc_begin("XBD.unistd.h.030");
	pc_test();
	if (!IS_T(&(crypt), char *(*)(const char *, const char *)))
		pc_fail("crypt() is not declared as char *crypt(const char *, const char *)");
	if (IS_T(&(crypt), char *(*)(const char *, const char *, int)))
		pc_fail("crypt() is declared without a prototype");
	if (!IS_T(&(encrypt), void (*)(char [64], int)))
		pc_fail("encrypt() is not declared as void encrypt(char [64], int)");
	if (IS_T(&(encrypt), void (*)(char [64], int, int)))
		pc_fail("encrypt() is declared without a prototype");
	pc_pass();
}
