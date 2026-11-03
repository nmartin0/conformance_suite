/*
 * <search.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.search.h.005
 *
 * Only <search.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <search.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.search.h.005");
	pc_test();
	if (!IS_T(&(hcreate), int (*)(size_t)))
		pc_fail("hcreate() is not declared as int hcreate(size_t)");
	if (IS_T(&(hcreate), int (*)(size_t, int)))
		pc_fail("hcreate() is declared without a prototype");
	if (!IS_T(&(hdestroy), void (*)(void)))
		pc_fail("hdestroy() is not declared as void hdestroy(void)");
	if (IS_T(&(hdestroy), void (*)(int)))
		pc_fail("hdestroy() is declared without a prototype");
	if (!IS_T(&(hsearch), ENTRY *(*)(ENTRY, ACTION)))
		pc_fail("hsearch() is not declared as ENTRY *hsearch(ENTRY, ACTION)");
	if (IS_T(&(hsearch), ENTRY *(*)(ENTRY, ACTION, int)))
		pc_fail("hsearch() is declared without a prototype");
	if (!IS_T(&(insque), void (*)(void *, void *)))
		pc_fail("insque() is not declared as void insque(void *, void *)");
	if (IS_T(&(insque), void (*)(void *, void *, int)))
		pc_fail("insque() is declared without a prototype");
	if (!IS_T(&(lfind), void *(*)(const void *, const void *, size_t *, size_t, int (*)(const void *, const void *))))
		pc_fail("lfind() is not declared as void *lfind(const void *, const void *, size_t *, size_t, int (*)(const void *, const void *))");
	if (IS_T(&(lfind), void *(*)(const void *, const void *, size_t *, size_t, int (*)(const void *, const void *), int)))
		pc_fail("lfind() is declared without a prototype");
	if (!IS_T(&(lsearch), void *(*)(const void *, void *, size_t *, size_t, int (*)(const void *, const void *))))
		pc_fail("lsearch() is not declared as void *lsearch(const void *, void *, size_t *, size_t, int (*)(const void *, const void *))");
	if (IS_T(&(lsearch), void *(*)(const void *, void *, size_t *, size_t, int (*)(const void *, const void *), int)))
		pc_fail("lsearch() is declared without a prototype");
	if (!IS_T(&(remque), void (*)(void *)))
		pc_fail("remque() is not declared as void remque(void *)");
	if (IS_T(&(remque), void (*)(void *, int)))
		pc_fail("remque() is declared without a prototype");
	if (!IS_T(&(tdelete), void *(*)(const void *, posix_tnode **, int (*)(const void *, const void *))))
		pc_fail("tdelete() is not declared as void *tdelete(const void *restrict, posix_tnode **restrict, int (*)(const void *, const void *))");
	if (IS_T(&(tdelete), void *(*)(const void *, posix_tnode **, int (*)(const void *, const void *), int)))
		pc_fail("tdelete() is declared without a prototype");
	if (!IS_T(&(tfind), posix_tnode *(*)(const void *, posix_tnode *const *, int (*)(const void *, const void *))))
		pc_fail("tfind() is not declared as posix_tnode *tfind(const void *, posix_tnode *const *, int (*)(const void *, const void *))");
	if (IS_T(&(tfind), posix_tnode *(*)(const void *, posix_tnode *const *, int (*)(const void *, const void *), int)))
		pc_fail("tfind() is declared without a prototype");
	if (!IS_T(&(tsearch), posix_tnode *(*)(const void *, posix_tnode **, int (*)(const void *, const void *))))
		pc_fail("tsearch() is not declared as posix_tnode *tsearch(const void *, posix_tnode **, int (*)(const void *, const void *))");
	if (IS_T(&(tsearch), posix_tnode *(*)(const void *, posix_tnode **, int (*)(const void *, const void *), int)))
		pc_fail("tsearch() is declared without a prototype");
	if (!IS_T(&(twalk), void (*)(const posix_tnode *, void (*)(const posix_tnode *, VISIT, int))))
		pc_fail("twalk() is not declared as void twalk(const posix_tnode *, void (*)(const posix_tnode *, VISIT, int))");
	if (IS_T(&(twalk), void (*)(const posix_tnode *, void (*)(const posix_tnode *, VISIT, int), int)))
		pc_fail("twalk() is declared without a prototype");
	pc_pass();
}
