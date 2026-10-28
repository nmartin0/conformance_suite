/*
 * <dlfcn.h> struct dl_info_t
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.dlfcn.h.002
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

typedef char pc_c0[sizeof(Dl_info_t)];	/* a complete type */
static Dl_info_t *ps;

int main(void)
{
	pc_begin("XBD.dlfcn.h.002");
	pc_test();
	(void)ps;
	if (!IS_T(&ps->dli_fname, const char * *))
		pc_fail("Dl_info_t member dli_fname is not of type const char *");
	if (!IS_T(&ps->dli_fbase, void * *))
		pc_fail("Dl_info_t member dli_fbase is not of type void *");
	if (!IS_T(&ps->dli_sname, const char * *))
		pc_fail("Dl_info_t member dli_sname is not of type const char *");
	if (!IS_T(&ps->dli_saddr, void * *))
		pc_fail("Dl_info_t member dli_saddr is not of type void *");
	pc_pass();
}
