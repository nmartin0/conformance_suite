/*
 * <wchar.h> types base
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.wchar.h.006
 *
 * Only <wchar.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <wchar.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef FILE *pc_t0;
typedef locale_t *pc_t1;
typedef mbstate_t *pc_t2;
typedef size_t *pc_t3;
typedef va_list *pc_t4;
typedef wchar_t *pc_t5;
typedef wint_t *pc_t6;

int main(void)
{
	pc_begin("XBD.wchar.h.006");
	pc_test();
	pc_pass();
}
