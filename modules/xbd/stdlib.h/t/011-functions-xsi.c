/*
 * <stdlib.h> functions xsi
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdlib.h.011
 *
 * Only <stdlib.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <stdlib.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.stdlib.h.011");
	pc_test();
	if (!IS_T(&(a64l), long (*)(const char *)))
		pc_fail("a64l() is not declared as long a64l(const char *)");
	if (IS_T(&(a64l), long (*)(const char *, int)))
		pc_fail("a64l() is declared without a prototype");
	if (!IS_T(&(drand48), double (*)(void)))
		pc_fail("drand48() is not declared as double drand48(void)");
	if (IS_T(&(drand48), double (*)(int)))
		pc_fail("drand48() is declared without a prototype");
	if (!IS_T(&(erand48), double (*)(unsigned short [3])))
		pc_fail("erand48() is not declared as double erand48(unsigned short [3])");
	if (IS_T(&(erand48), double (*)(unsigned short [3], int)))
		pc_fail("erand48() is declared without a prototype");
	if (!IS_T(&(grantpt), int (*)(int)))
		pc_fail("grantpt() is not declared as int grantpt(int)");
	if (IS_T(&(grantpt), int (*)(int, int)))
		pc_fail("grantpt() is declared without a prototype");
	if (!IS_T(&(initstate), char *(*)(unsigned, char *, size_t)))
		pc_fail("initstate() is not declared as char *initstate(unsigned, char *, size_t)");
	if (IS_T(&(initstate), char *(*)(unsigned, char *, size_t, int)))
		pc_fail("initstate() is declared without a prototype");
	if (!IS_T(&(jrand48), long (*)(unsigned short [3])))
		pc_fail("jrand48() is not declared as long jrand48(unsigned short [3])");
	if (IS_T(&(jrand48), long (*)(unsigned short [3], int)))
		pc_fail("jrand48() is declared without a prototype");
	if (!IS_T(&(l64a), char *(*)(long)))
		pc_fail("l64a() is not declared as char *l64a(long)");
	if (IS_T(&(l64a), char *(*)(long, int)))
		pc_fail("l64a() is declared without a prototype");
	if (!IS_T(&(lcong48), void (*)(unsigned short [7])))
		pc_fail("lcong48() is not declared as void lcong48(unsigned short [7])");
	if (IS_T(&(lcong48), void (*)(unsigned short [7], int)))
		pc_fail("lcong48() is declared without a prototype");
	if (!IS_T(&(lrand48), long (*)(void)))
		pc_fail("lrand48() is not declared as long lrand48(void)");
	if (IS_T(&(lrand48), long (*)(int)))
		pc_fail("lrand48() is declared without a prototype");
	if (!IS_T(&(mrand48), long (*)(void)))
		pc_fail("mrand48() is not declared as long mrand48(void)");
	if (IS_T(&(mrand48), long (*)(int)))
		pc_fail("mrand48() is declared without a prototype");
	if (!IS_T(&(nrand48), long (*)(unsigned short [3])))
		pc_fail("nrand48() is not declared as long nrand48(unsigned short [3])");
	if (IS_T(&(nrand48), long (*)(unsigned short [3], int)))
		pc_fail("nrand48() is declared without a prototype");
	if (!IS_T(&(posix_openpt), int (*)(int)))
		pc_fail("posix_openpt() is not declared as int posix_openpt(int)");
	if (IS_T(&(posix_openpt), int (*)(int, int)))
		pc_fail("posix_openpt() is declared without a prototype");
	if (!IS_T(&(ptsname), char *(*)(int)))
		pc_fail("ptsname() is not declared as char *ptsname(int)");
	if (IS_T(&(ptsname), char *(*)(int, int)))
		pc_fail("ptsname() is declared without a prototype");
	if (!IS_T(&(putenv), int (*)(char *)))
		pc_fail("putenv() is not declared as int putenv(char *)");
	if (IS_T(&(putenv), int (*)(char *, int)))
		pc_fail("putenv() is declared without a prototype");
	if (!IS_T(&(random), long (*)(void)))
		pc_fail("random() is not declared as long random(void)");
	if (IS_T(&(random), long (*)(int)))
		pc_fail("random() is declared without a prototype");
	if (!IS_T(&(seed48), unsigned short *(*)(unsigned short [3])))
		pc_fail("seed48() is not declared as unsigned short *seed48(unsigned short [3])");
	if (IS_T(&(seed48), unsigned short *(*)(unsigned short [3], int)))
		pc_fail("seed48() is declared without a prototype");
	if (!IS_T(&(setstate), char *(*)(char *)))
		pc_fail("setstate() is not declared as char *setstate(char *)");
	if (IS_T(&(setstate), char *(*)(char *, int)))
		pc_fail("setstate() is declared without a prototype");
	if (!IS_T(&(srand48), void (*)(long)))
		pc_fail("srand48() is not declared as void srand48(long)");
	if (IS_T(&(srand48), void (*)(long, int)))
		pc_fail("srand48() is declared without a prototype");
	if (!IS_T(&(srandom), void (*)(unsigned)))
		pc_fail("srandom() is not declared as void srandom(unsigned)");
	if (IS_T(&(srandom), void (*)(unsigned, int)))
		pc_fail("srandom() is declared without a prototype");
	if (!IS_T(&(unlockpt), int (*)(int)))
		pc_fail("unlockpt() is not declared as int unlockpt(int)");
	if (IS_T(&(unlockpt), int (*)(int, int)))
		pc_fail("unlockpt() is declared without a prototype");
	pc_pass();
}
