/*
 * <ndbm.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.ndbm.h.005
 *
 * Only <ndbm.h> is included (not pc.h, which brings in other headers), so a
 * name the header fails to provide stops the build, which the module
 * reports as FAIL.
 */
#include <ndbm.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.ndbm.h.005");
	pc_test();
	if (!IS_T(&(dbm_clearerr), int (*)(DBM *)))
		pc_fail("dbm_clearerr() is not declared with the prototype the page gives");
	if (!IS_T(&(dbm_close), void (*)(DBM *)))
		pc_fail("dbm_close() is not declared with the prototype the page gives");
	if (!IS_T(&(dbm_delete), int (*)(DBM *, datum)))
		pc_fail("dbm_delete() is not declared with the prototype the page gives");
	if (!IS_T(&(dbm_error), int (*)(DBM *)))
		pc_fail("dbm_error() is not declared with the prototype the page gives");
	if (!IS_T(&(dbm_fetch), datum (*)(DBM *, datum)))
		pc_fail("dbm_fetch() is not declared with the prototype the page gives");
	if (!IS_T(&(dbm_firstkey), datum (*)(DBM *)))
		pc_fail("dbm_firstkey() is not declared with the prototype the page gives");
	if (!IS_T(&(dbm_nextkey), datum (*)(DBM *)))
		pc_fail("dbm_nextkey() is not declared with the prototype the page gives");
	if (!IS_T(&(dbm_open), DBM *(*)(const char *, int, mode_t)))
		pc_fail("dbm_open() is not declared with the prototype the page gives");
	if (!IS_T(&(dbm_store), int (*)(DBM *, datum, datum, int)))
		pc_fail("dbm_store() is not declared with the prototype the page gives");
	pc_pass();
}
