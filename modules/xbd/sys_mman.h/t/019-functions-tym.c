/*
 * <sys/mman.h> functions tym
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_mman.h.019
 *
 * Only <sys/mman.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/mman.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.sys_mman.h.019");
	pc_test();
	if (!IS_T(&(posix_mem_offset), int (*)(const void *restrict, size_t, off_t *restrict, size_t *restrict, int *restrict)))
		pc_fail("posix_mem_offset() is not declared as int posix_mem_offset(const void *restrict, size_t, off_t *restrict, size_t *restrict, int *restrict)");
	if (IS_T(&(posix_mem_offset), int (*)(const void *restrict, size_t, off_t *restrict, size_t *restrict, int *restrict, int)))
		pc_fail("posix_mem_offset() is declared without a prototype");
	if (!IS_T(&(posix_typed_mem_get_info), int (*)(int, struct posix_typed_mem_info *)))
		pc_fail("posix_typed_mem_get_info() is not declared as int posix_typed_mem_get_info(int, struct posix_typed_mem_info *)");
	if (IS_T(&(posix_typed_mem_get_info), int (*)(int, struct posix_typed_mem_info *, int)))
		pc_fail("posix_typed_mem_get_info() is declared without a prototype");
	if (!IS_T(&(posix_typed_mem_open), int (*)(const char *, int, int)))
		pc_fail("posix_typed_mem_open() is not declared as int posix_typed_mem_open(const char *, int, int)");
	if (IS_T(&(posix_typed_mem_open), int (*)(const char *, int, int, int)))
		pc_fail("posix_typed_mem_open() is declared without a prototype");
	pc_pass();
}
