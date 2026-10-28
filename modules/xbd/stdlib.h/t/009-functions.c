/*
 * <stdlib.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdlib.h.009
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
	pc_begin("XBD.stdlib.h.009");
	pc_test();
	if (!IS_T(&(_Exit), void (*)(int)))
		pc_fail("_Exit() is not declared as _Noreturn void _Exit(int)");
	if (IS_T(&(_Exit), void (*)(int, int)))
		pc_fail("_Exit() is declared without a prototype");
	if (!IS_T(&(abort), void (*)(void)))
		pc_fail("abort() is not declared as _Noreturn void abort(void)");
	if (IS_T(&(abort), void (*)(int)))
		pc_fail("abort() is declared without a prototype");
	if (!IS_T(&(abs), int (*)(int)))
		pc_fail("abs() is not declared as int abs(int)");
	if (IS_T(&(abs), int (*)(int, int)))
		pc_fail("abs() is declared without a prototype");
	if (!IS_T(&(aligned_alloc), void *(*)(size_t, size_t)))
		pc_fail("aligned_alloc() is not declared as void *aligned_alloc(size_t, size_t)");
	if (IS_T(&(aligned_alloc), void *(*)(size_t, size_t, int)))
		pc_fail("aligned_alloc() is declared without a prototype");
	if (!IS_T(&(at_quick_exit), int (*)(void (*)(void))))
		pc_fail("at_quick_exit() is not declared as int at_quick_exit(void (*)(void))");
	if (IS_T(&(at_quick_exit), int (*)(void (*)(void), int)))
		pc_fail("at_quick_exit() is declared without a prototype");
	if (!IS_T(&(atexit), int (*)(void (*)(void))))
		pc_fail("atexit() is not declared as int atexit(void (*)(void))");
	if (IS_T(&(atexit), int (*)(void (*)(void), int)))
		pc_fail("atexit() is declared without a prototype");
	if (!IS_T(&(atof), double (*)(const char *)))
		pc_fail("atof() is not declared as double atof(const char *)");
	if (IS_T(&(atof), double (*)(const char *, int)))
		pc_fail("atof() is declared without a prototype");
	if (!IS_T(&(atoi), int (*)(const char *)))
		pc_fail("atoi() is not declared as int atoi(const char *)");
	if (IS_T(&(atoi), int (*)(const char *, int)))
		pc_fail("atoi() is declared without a prototype");
	if (!IS_T(&(atol), long (*)(const char *)))
		pc_fail("atol() is not declared as long atol(const char *)");
	if (IS_T(&(atol), long (*)(const char *, int)))
		pc_fail("atol() is declared without a prototype");
	if (!IS_T(&(atoll), long long (*)(const char *)))
		pc_fail("atoll() is not declared as long long atoll(const char *)");
	if (IS_T(&(atoll), long long (*)(const char *, int)))
		pc_fail("atoll() is declared without a prototype");
	if (!IS_T(&(bsearch), void *(*)(const void *, const void *, size_t, size_t, int (*)(const void *, const void *))))
		pc_fail("bsearch() is not declared as void *bsearch(const void *, const void *, size_t, size_t, int (*)(const void *, const void *))");
	if (IS_T(&(bsearch), void *(*)(const void *, const void *, size_t, size_t, int (*)(const void *, const void *), int)))
		pc_fail("bsearch() is declared without a prototype");
	if (!IS_T(&(calloc), void *(*)(size_t, size_t)))
		pc_fail("calloc() is not declared as void *calloc(size_t, size_t)");
	if (IS_T(&(calloc), void *(*)(size_t, size_t, int)))
		pc_fail("calloc() is declared without a prototype");
	if (!IS_T(&(div), div_t (*)(int, int)))
		pc_fail("div() is not declared as div_t div(int, int)");
	if (IS_T(&(div), div_t (*)(int, int, int)))
		pc_fail("div() is declared without a prototype");
	if (!IS_T(&(exit), void (*)(int)))
		pc_fail("exit() is not declared as _Noreturn void exit(int)");
	if (IS_T(&(exit), void (*)(int, int)))
		pc_fail("exit() is declared without a prototype");
	if (!IS_T(&(free), void (*)(void *)))
		pc_fail("free() is not declared as void free(void *)");
	if (IS_T(&(free), void (*)(void *, int)))
		pc_fail("free() is declared without a prototype");
	if (!IS_T(&(getenv), char *(*)(const char *)))
		pc_fail("getenv() is not declared as char *getenv(const char *)");
	if (IS_T(&(getenv), char *(*)(const char *, int)))
		pc_fail("getenv() is declared without a prototype");
	if (!IS_T(&(getsubopt), int (*)(char **restrict, char *const *restrict, char **restrict)))
		pc_fail("getsubopt() is not declared as int getsubopt(char **restrict, char *const *restrict, char **restrict)");
	if (IS_T(&(getsubopt), int (*)(char **restrict, char *const *restrict, char **restrict, int)))
		pc_fail("getsubopt() is declared without a prototype");
	if (!IS_T(&(labs), long (*)(long)))
		pc_fail("labs() is not declared as long labs(long)");
	if (IS_T(&(labs), long (*)(long, int)))
		pc_fail("labs() is declared without a prototype");
	if (!IS_T(&(ldiv), ldiv_t (*)(long, long)))
		pc_fail("ldiv() is not declared as ldiv_t ldiv(long, long)");
	if (IS_T(&(ldiv), ldiv_t (*)(long, long, int)))
		pc_fail("ldiv() is declared without a prototype");
	if (!IS_T(&(llabs), long long (*)(long long)))
		pc_fail("llabs() is not declared as long long llabs(long long)");
	if (IS_T(&(llabs), long long (*)(long long, int)))
		pc_fail("llabs() is declared without a prototype");
	if (!IS_T(&(lldiv), lldiv_t (*)(long long, long long)))
		pc_fail("lldiv() is not declared as lldiv_t lldiv(long long, long long)");
	if (IS_T(&(lldiv), lldiv_t (*)(long long, long long, int)))
		pc_fail("lldiv() is declared without a prototype");
	if (!IS_T(&(malloc), void *(*)(size_t)))
		pc_fail("malloc() is not declared as void *malloc(size_t)");
	if (IS_T(&(malloc), void *(*)(size_t, int)))
		pc_fail("malloc() is declared without a prototype");
	if (!IS_T(&(mblen), int (*)(const char *, size_t)))
		pc_fail("mblen() is not declared as int mblen(const char *, size_t)");
	if (IS_T(&(mblen), int (*)(const char *, size_t, int)))
		pc_fail("mblen() is declared without a prototype");
	if (!IS_T(&(mbstowcs), size_t (*)(wchar_t *restrict, const char *restrict, size_t)))
		pc_fail("mbstowcs() is not declared as size_t mbstowcs(wchar_t *restrict, const char *restrict, size_t)");
	if (IS_T(&(mbstowcs), size_t (*)(wchar_t *restrict, const char *restrict, size_t, int)))
		pc_fail("mbstowcs() is declared without a prototype");
	if (!IS_T(&(mbtowc), int (*)(wchar_t *restrict, const char *restrict, size_t)))
		pc_fail("mbtowc() is not declared as int mbtowc(wchar_t *restrict, const char *restrict, size_t)");
	if (IS_T(&(mbtowc), int (*)(wchar_t *restrict, const char *restrict, size_t, int)))
		pc_fail("mbtowc() is declared without a prototype");
	if (!IS_T(&(mkdtemp), char *(*)(char *)))
		pc_fail("mkdtemp() is not declared as char *mkdtemp(char *)");
	if (IS_T(&(mkdtemp), char *(*)(char *, int)))
		pc_fail("mkdtemp() is declared without a prototype");
	if (!IS_T(&(mkstemp), int (*)(char *)))
		pc_fail("mkstemp() is not declared as int mkstemp(char *)");
	if (IS_T(&(mkstemp), int (*)(char *, int)))
		pc_fail("mkstemp() is declared without a prototype");
	if (!IS_T(&(qsort), void (*)(void *, size_t, size_t, int (*)(const void *, const void *))))
		pc_fail("qsort() is not declared as void qsort(void *, size_t, size_t, int (*)(const void *, const void *))");
	if (IS_T(&(qsort), void (*)(void *, size_t, size_t, int (*)(const void *, const void *), int)))
		pc_fail("qsort() is declared without a prototype");
	if (!IS_T(&(quick_exit), void (*)(int)))
		pc_fail("quick_exit() is not declared as _Noreturn void quick_exit(int)");
	if (IS_T(&(quick_exit), void (*)(int, int)))
		pc_fail("quick_exit() is declared without a prototype");
	if (!IS_T(&(rand), int (*)(void)))
		pc_fail("rand() is not declared as int rand(void)");
	if (IS_T(&(rand), int (*)(int)))
		pc_fail("rand() is declared without a prototype");
	if (!IS_T(&(realloc), void *(*)(void *, size_t)))
		pc_fail("realloc() is not declared as void *realloc(void *, size_t)");
	if (IS_T(&(realloc), void *(*)(void *, size_t, int)))
		pc_fail("realloc() is declared without a prototype");
	if (!IS_T(&(realpath), char *(*)(const char *restrict, char *restrict)))
		pc_fail("realpath() is not declared as char *realpath(const char *restrict, char *restrict)");
	if (IS_T(&(realpath), char *(*)(const char *restrict, char *restrict, int)))
		pc_fail("realpath() is declared without a prototype");
	if (!IS_T(&(setenv), int (*)(const char *, const char *, int)))
		pc_fail("setenv() is not declared as int setenv(const char *, const char *, int)");
	if (IS_T(&(setenv), int (*)(const char *, const char *, int, int)))
		pc_fail("setenv() is declared without a prototype");
	if (!IS_T(&(srand), void (*)(unsigned)))
		pc_fail("srand() is not declared as void srand(unsigned)");
	if (IS_T(&(srand), void (*)(unsigned, int)))
		pc_fail("srand() is declared without a prototype");
	if (!IS_T(&(strtod), double (*)(const char *restrict, char **restrict)))
		pc_fail("strtod() is not declared as double strtod(const char *restrict, char **restrict)");
	if (IS_T(&(strtod), double (*)(const char *restrict, char **restrict, int)))
		pc_fail("strtod() is declared without a prototype");
	if (!IS_T(&(strtof), float (*)(const char *restrict, char **restrict)))
		pc_fail("strtof() is not declared as float strtof(const char *restrict, char **restrict)");
	if (IS_T(&(strtof), float (*)(const char *restrict, char **restrict, int)))
		pc_fail("strtof() is declared without a prototype");
	if (!IS_T(&(strtol), long (*)(const char *restrict, char **restrict, int)))
		pc_fail("strtol() is not declared as long strtol(const char *restrict, char **restrict, int)");
	if (IS_T(&(strtol), long (*)(const char *restrict, char **restrict, int, int)))
		pc_fail("strtol() is declared without a prototype");
	if (!IS_T(&(strtold), long double (*)(const char *restrict, char **restrict)))
		pc_fail("strtold() is not declared as long double strtold(const char *restrict, char **restrict)");
	if (IS_T(&(strtold), long double (*)(const char *restrict, char **restrict, int)))
		pc_fail("strtold() is declared without a prototype");
	if (!IS_T(&(strtoll), long long (*)(const char *restrict, char **restrict, int)))
		pc_fail("strtoll() is not declared as long long strtoll(const char *restrict, char **restrict, int)");
	if (IS_T(&(strtoll), long long (*)(const char *restrict, char **restrict, int, int)))
		pc_fail("strtoll() is declared without a prototype");
	if (!IS_T(&(strtoul), unsigned long (*)(const char *restrict, char **restrict, int)))
		pc_fail("strtoul() is not declared as unsigned long strtoul(const char *restrict, char **restrict, int)");
	if (IS_T(&(strtoul), unsigned long (*)(const char *restrict, char **restrict, int, int)))
		pc_fail("strtoul() is declared without a prototype");
	if (!IS_T(&(strtoull), unsigned long long (*)(const char *restrict, char **restrict, int)))
		pc_fail("strtoull() is not declared as unsigned long long strtoull(const char *restrict, char **restrict, int)");
	if (IS_T(&(strtoull), unsigned long long (*)(const char *restrict, char **restrict, int, int)))
		pc_fail("strtoull() is declared without a prototype");
	if (!IS_T(&(system), int (*)(const char *)))
		pc_fail("system() is not declared as int system(const char *)");
	if (IS_T(&(system), int (*)(const char *, int)))
		pc_fail("system() is declared without a prototype");
	if (!IS_T(&(unsetenv), int (*)(const char *)))
		pc_fail("unsetenv() is not declared as int unsetenv(const char *)");
	if (IS_T(&(unsetenv), int (*)(const char *, int)))
		pc_fail("unsetenv() is declared without a prototype");
	if (!IS_T(&(wcstombs), size_t (*)(char *restrict, const wchar_t *restrict, size_t)))
		pc_fail("wcstombs() is not declared as size_t wcstombs(char *restrict, const wchar_t *restrict, size_t)");
	if (IS_T(&(wcstombs), size_t (*)(char *restrict, const wchar_t *restrict, size_t, int)))
		pc_fail("wcstombs() is declared without a prototype");
	if (!IS_T(&(wctomb), int (*)(char *, wchar_t)))
		pc_fail("wctomb() is not declared as int wctomb(char *, wchar_t)");
	if (IS_T(&(wctomb), int (*)(char *, wchar_t, int)))
		pc_fail("wctomb() is declared without a prototype");
	pc_pass();
}
