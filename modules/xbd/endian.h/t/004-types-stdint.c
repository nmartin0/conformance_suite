/*
 * <endian.h> types uint16_t uint32_t uint64_t
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.endian.h.004
 *
 * Only <endian.h> is included (not pc.h, which brings in other headers), so a
 * name the header fails to provide stops the build, which the module
 * reports as FAIL.
 */
#include <endian.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

typedef uint16_t *pc_t0;
typedef uint32_t *pc_t1;
typedef uint64_t *pc_t2;

int main(void)
{
	pc_begin("XBD.endian.h.004");
	pc_test();
	pc_pass();
}
