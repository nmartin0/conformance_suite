/*
 * <dlfcn.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.dlfcn.h.003
 *
 * Only <dlfcn.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <dlfcn.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.dlfcn.h.003");
	pc_test();
	if (!IS_T(&(dlclose), int (*)(void *)))
		pc_fail("dlclose() is not declared as int dlclose(void *)");
	if (IS_T(&(dlclose), int (*)(void *, int)))
		pc_fail("dlclose() is declared without a prototype");
	if (!IS_T(&(dlerror), char *(*)(void)))
		pc_fail("dlerror() is not declared as char *dlerror(void)");
	if (IS_T(&(dlerror), char *(*)(int)))
		pc_fail("dlerror() is declared without a prototype");
	if (!IS_T(&(dlopen), void *(*)(const char *, int)))
		pc_fail("dlopen() is not declared as void *dlopen(const char *, int)");
	if (IS_T(&(dlopen), void *(*)(const char *, int, int)))
		pc_fail("dlopen() is declared without a prototype");
	if (!IS_T(&(dlsym), void *(*)(void *restrict, const char *restrict)))
		pc_fail("dlsym() is not declared as void *dlsym(void *restrict, const char *restrict)");
	if (IS_T(&(dlsym), void *(*)(void *restrict, const char *restrict, int)))
		pc_fail("dlsym() is declared without a prototype");
	pc_pass();
}
