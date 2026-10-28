/*
 * <wchar.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.wchar.h.009
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

int main(void)
{
	pc_begin("XBD.wchar.h.009");
	pc_test();
	if (!IS_T(&(btowc), wint_t (*)(int)))
		pc_fail("btowc() is not declared as wint_t btowc(int)");
	if (IS_T(&(btowc), wint_t (*)(int, int)))
		pc_fail("btowc() is declared without a prototype");
	if (!IS_T(&(fgetwc), wint_t (*)(FILE *)))
		pc_fail("fgetwc() is not declared as wint_t fgetwc(FILE *)");
	if (IS_T(&(fgetwc), wint_t (*)(FILE *, int)))
		pc_fail("fgetwc() is declared without a prototype");
	if (!IS_T(&(fgetws), wchar_t *(*)(wchar_t *restrict, int, FILE *restrict)))
		pc_fail("fgetws() is not declared as wchar_t *fgetws(wchar_t *restrict, int, FILE *restrict)");
	if (IS_T(&(fgetws), wchar_t *(*)(wchar_t *restrict, int, FILE *restrict, int)))
		pc_fail("fgetws() is declared without a prototype");
	if (!IS_T(&(fputwc), wint_t (*)(wchar_t, FILE *)))
		pc_fail("fputwc() is not declared as wint_t fputwc(wchar_t, FILE *)");
	if (IS_T(&(fputwc), wint_t (*)(wchar_t, FILE *, int)))
		pc_fail("fputwc() is declared without a prototype");
	if (!IS_T(&(fputws), int (*)(const wchar_t *restrict, FILE *restrict)))
		pc_fail("fputws() is not declared as int fputws(const wchar_t *restrict, FILE *restrict)");
	if (IS_T(&(fputws), int (*)(const wchar_t *restrict, FILE *restrict, int)))
		pc_fail("fputws() is declared without a prototype");
	if (!IS_T(&(fwide), int (*)(FILE *, int)))
		pc_fail("fwide() is not declared as int fwide(FILE *, int)");
	if (IS_T(&(fwide), int (*)(FILE *, int, int)))
		pc_fail("fwide() is declared without a prototype");
	if (!IS_T(&(fwprintf), int (*)(FILE *restrict, const wchar_t *restrict, ...)))
		pc_fail("fwprintf() is not declared as int fwprintf(FILE *restrict, const wchar_t *restrict, ...)");
	if (!IS_T(&(fwscanf), int (*)(FILE *restrict, const wchar_t *restrict, ...)))
		pc_fail("fwscanf() is not declared as int fwscanf(FILE *restrict, const wchar_t *restrict, ...)");
	if (!IS_T(&(getwc), wint_t (*)(FILE *)))
		pc_fail("getwc() is not declared as wint_t getwc(FILE *)");
	if (IS_T(&(getwc), wint_t (*)(FILE *, int)))
		pc_fail("getwc() is declared without a prototype");
	if (!IS_T(&(getwchar), wint_t (*)(void)))
		pc_fail("getwchar() is not declared as wint_t getwchar(void)");
	if (IS_T(&(getwchar), wint_t (*)(int)))
		pc_fail("getwchar() is declared without a prototype");
	if (!IS_T(&(mbrlen), size_t (*)(const char *restrict, size_t, mbstate_t *restrict)))
		pc_fail("mbrlen() is not declared as size_t mbrlen(const char *restrict, size_t, mbstate_t *restrict)");
	if (IS_T(&(mbrlen), size_t (*)(const char *restrict, size_t, mbstate_t *restrict, int)))
		pc_fail("mbrlen() is declared without a prototype");
	if (!IS_T(&(mbrtowc), size_t (*)(wchar_t *restrict, const char *restrict, size_t, mbstate_t *restrict)))
		pc_fail("mbrtowc() is not declared as size_t mbrtowc(wchar_t *restrict, const char *restrict, size_t, mbstate_t *restrict)");
	if (IS_T(&(mbrtowc), size_t (*)(wchar_t *restrict, const char *restrict, size_t, mbstate_t *restrict, int)))
		pc_fail("mbrtowc() is declared without a prototype");
	if (!IS_T(&(mbsinit), int (*)(const mbstate_t *)))
		pc_fail("mbsinit() is not declared as int mbsinit(const mbstate_t *)");
	if (IS_T(&(mbsinit), int (*)(const mbstate_t *, int)))
		pc_fail("mbsinit() is declared without a prototype");
	if (!IS_T(&(mbsnrtowcs), size_t (*)(wchar_t *restrict, const char **restrict, size_t, size_t, mbstate_t *restrict)))
		pc_fail("mbsnrtowcs() is not declared as size_t mbsnrtowcs(wchar_t *restrict, const char **restrict, size_t, size_t, mbstate_t *restrict)");
	if (IS_T(&(mbsnrtowcs), size_t (*)(wchar_t *restrict, const char **restrict, size_t, size_t, mbstate_t *restrict, int)))
		pc_fail("mbsnrtowcs() is declared without a prototype");
	if (!IS_T(&(mbsrtowcs), size_t (*)(wchar_t *restrict, const char **restrict, size_t, mbstate_t *restrict)))
		pc_fail("mbsrtowcs() is not declared as size_t mbsrtowcs(wchar_t *restrict, const char **restrict, size_t, mbstate_t *restrict)");
	if (IS_T(&(mbsrtowcs), size_t (*)(wchar_t *restrict, const char **restrict, size_t, mbstate_t *restrict, int)))
		pc_fail("mbsrtowcs() is declared without a prototype");
	if (!IS_T(&(open_wmemstream), FILE *(*)(wchar_t **, size_t *)))
		pc_fail("open_wmemstream() is not declared as FILE *open_wmemstream(wchar_t **, size_t *)");
	if (IS_T(&(open_wmemstream), FILE *(*)(wchar_t **, size_t *, int)))
		pc_fail("open_wmemstream() is declared without a prototype");
	if (!IS_T(&(putwc), wint_t (*)(wchar_t, FILE *)))
		pc_fail("putwc() is not declared as wint_t putwc(wchar_t, FILE *)");
	if (IS_T(&(putwc), wint_t (*)(wchar_t, FILE *, int)))
		pc_fail("putwc() is declared without a prototype");
	if (!IS_T(&(putwchar), wint_t (*)(wchar_t)))
		pc_fail("putwchar() is not declared as wint_t putwchar(wchar_t)");
	if (IS_T(&(putwchar), wint_t (*)(wchar_t, int)))
		pc_fail("putwchar() is declared without a prototype");
	if (!IS_T(&(swprintf), int (*)(wchar_t *restrict, size_t, const wchar_t *restrict, ...)))
		pc_fail("swprintf() is not declared as int swprintf(wchar_t *restrict, size_t, const wchar_t *restrict, ...)");
	if (!IS_T(&(swscanf), int (*)(const wchar_t *restrict, const wchar_t *restrict, ...)))
		pc_fail("swscanf() is not declared as int swscanf(const wchar_t *restrict, const wchar_t *restrict, ...)");
	if (!IS_T(&(ungetwc), wint_t (*)(wint_t, FILE *)))
		pc_fail("ungetwc() is not declared as wint_t ungetwc(wint_t, FILE *)");
	if (IS_T(&(ungetwc), wint_t (*)(wint_t, FILE *, int)))
		pc_fail("ungetwc() is declared without a prototype");
	if (!IS_T(&(vfwprintf), int (*)(FILE *restrict, const wchar_t *restrict, va_list)))
		pc_fail("vfwprintf() is not declared as int vfwprintf(FILE *restrict, const wchar_t *restrict, va_list)");
	if (IS_T(&(vfwprintf), int (*)(FILE *restrict, const wchar_t *restrict, va_list, int)))
		pc_fail("vfwprintf() is declared without a prototype");
	if (!IS_T(&(vfwscanf), int (*)(FILE *restrict, const wchar_t *restrict, va_list)))
		pc_fail("vfwscanf() is not declared as int vfwscanf(FILE *restrict, const wchar_t *restrict, va_list)");
	if (IS_T(&(vfwscanf), int (*)(FILE *restrict, const wchar_t *restrict, va_list, int)))
		pc_fail("vfwscanf() is declared without a prototype");
	if (!IS_T(&(vswprintf), int (*)(wchar_t *restrict, size_t, const wchar_t *restrict, va_list)))
		pc_fail("vswprintf() is not declared as int vswprintf(wchar_t *restrict, size_t, const wchar_t *restrict, va_list)");
	if (IS_T(&(vswprintf), int (*)(wchar_t *restrict, size_t, const wchar_t *restrict, va_list, int)))
		pc_fail("vswprintf() is declared without a prototype");
	if (!IS_T(&(vswscanf), int (*)(const wchar_t *restrict, const wchar_t *restrict, va_list)))
		pc_fail("vswscanf() is not declared as int vswscanf(const wchar_t *restrict, const wchar_t *restrict, va_list)");
	if (IS_T(&(vswscanf), int (*)(const wchar_t *restrict, const wchar_t *restrict, va_list, int)))
		pc_fail("vswscanf() is declared without a prototype");
	if (!IS_T(&(vwprintf), int (*)(const wchar_t *restrict, va_list)))
		pc_fail("vwprintf() is not declared as int vwprintf(const wchar_t *restrict, va_list)");
	if (IS_T(&(vwprintf), int (*)(const wchar_t *restrict, va_list, int)))
		pc_fail("vwprintf() is declared without a prototype");
	if (!IS_T(&(vwscanf), int (*)(const wchar_t *restrict, va_list)))
		pc_fail("vwscanf() is not declared as int vwscanf(const wchar_t *restrict, va_list)");
	if (IS_T(&(vwscanf), int (*)(const wchar_t *restrict, va_list, int)))
		pc_fail("vwscanf() is declared without a prototype");
	if (!IS_T(&(wcpcpy), wchar_t *(*)(wchar_t *restrict, const wchar_t *restrict)))
		pc_fail("wcpcpy() is not declared as wchar_t *wcpcpy(wchar_t *restrict, const wchar_t *restrict)");
	if (IS_T(&(wcpcpy), wchar_t *(*)(wchar_t *restrict, const wchar_t *restrict, int)))
		pc_fail("wcpcpy() is declared without a prototype");
	if (!IS_T(&(wcpncpy), wchar_t *(*)(wchar_t *restrict, const wchar_t *restrict, size_t)))
		pc_fail("wcpncpy() is not declared as wchar_t *wcpncpy(wchar_t *restrict, const wchar_t *restrict, size_t)");
	if (IS_T(&(wcpncpy), wchar_t *(*)(wchar_t *restrict, const wchar_t *restrict, size_t, int)))
		pc_fail("wcpncpy() is declared without a prototype");
	if (!IS_T(&(wcrtomb), size_t (*)(char *restrict, wchar_t, mbstate_t *restrict)))
		pc_fail("wcrtomb() is not declared as size_t wcrtomb(char *restrict, wchar_t, mbstate_t *restrict)");
	if (IS_T(&(wcrtomb), size_t (*)(char *restrict, wchar_t, mbstate_t *restrict, int)))
		pc_fail("wcrtomb() is declared without a prototype");
	if (!IS_T(&(wcscasecmp), int (*)(const wchar_t *, const wchar_t *)))
		pc_fail("wcscasecmp() is not declared as int wcscasecmp(const wchar_t *, const wchar_t *)");
	if (IS_T(&(wcscasecmp), int (*)(const wchar_t *, const wchar_t *, int)))
		pc_fail("wcscasecmp() is declared without a prototype");
	if (!IS_T(&(wcscasecmp_l), int (*)(const wchar_t *, const wchar_t *, locale_t)))
		pc_fail("wcscasecmp_l() is not declared as int wcscasecmp_l(const wchar_t *, const wchar_t *, locale_t)");
	if (IS_T(&(wcscasecmp_l), int (*)(const wchar_t *, const wchar_t *, locale_t, int)))
		pc_fail("wcscasecmp_l() is declared without a prototype");
	if (!IS_T(&(wcscat), wchar_t *(*)(wchar_t *restrict, const wchar_t *restrict)))
		pc_fail("wcscat() is not declared as wchar_t *wcscat(wchar_t *restrict, const wchar_t *restrict)");
	if (IS_T(&(wcscat), wchar_t *(*)(wchar_t *restrict, const wchar_t *restrict, int)))
		pc_fail("wcscat() is declared without a prototype");
	if (!IS_T(&(wcschr), wchar_t *(*)(const wchar_t *, wchar_t)))
		pc_fail("wcschr() is not declared as wchar_t *wcschr(const wchar_t *, wchar_t)");
	if (IS_T(&(wcschr), wchar_t *(*)(const wchar_t *, wchar_t, int)))
		pc_fail("wcschr() is declared without a prototype");
	if (!IS_T(&(wcscmp), int (*)(const wchar_t *, const wchar_t *)))
		pc_fail("wcscmp() is not declared as int wcscmp(const wchar_t *, const wchar_t *)");
	if (IS_T(&(wcscmp), int (*)(const wchar_t *, const wchar_t *, int)))
		pc_fail("wcscmp() is declared without a prototype");
	if (!IS_T(&(wcscoll), int (*)(const wchar_t *, const wchar_t *)))
		pc_fail("wcscoll() is not declared as int wcscoll(const wchar_t *, const wchar_t *)");
	if (IS_T(&(wcscoll), int (*)(const wchar_t *, const wchar_t *, int)))
		pc_fail("wcscoll() is declared without a prototype");
	if (!IS_T(&(wcscoll_l), int (*)(const wchar_t *, const wchar_t *, locale_t)))
		pc_fail("wcscoll_l() is not declared as int wcscoll_l(const wchar_t *, const wchar_t *, locale_t)");
	if (IS_T(&(wcscoll_l), int (*)(const wchar_t *, const wchar_t *, locale_t, int)))
		pc_fail("wcscoll_l() is declared without a prototype");
	if (!IS_T(&(wcscpy), wchar_t *(*)(wchar_t *restrict, const wchar_t *restrict)))
		pc_fail("wcscpy() is not declared as wchar_t *wcscpy(wchar_t *restrict, const wchar_t *restrict)");
	if (IS_T(&(wcscpy), wchar_t *(*)(wchar_t *restrict, const wchar_t *restrict, int)))
		pc_fail("wcscpy() is declared without a prototype");
	if (!IS_T(&(wcscspn), size_t (*)(const wchar_t *, const wchar_t *)))
		pc_fail("wcscspn() is not declared as size_t wcscspn(const wchar_t *, const wchar_t *)");
	if (IS_T(&(wcscspn), size_t (*)(const wchar_t *, const wchar_t *, int)))
		pc_fail("wcscspn() is declared without a prototype");
	if (!IS_T(&(wcsdup), wchar_t *(*)(const wchar_t *)))
		pc_fail("wcsdup() is not declared as wchar_t *wcsdup(const wchar_t *)");
	if (IS_T(&(wcsdup), wchar_t *(*)(const wchar_t *, int)))
		pc_fail("wcsdup() is declared without a prototype");
	if (!IS_T(&(wcsftime), size_t (*)(wchar_t *restrict, size_t, const wchar_t *restrict, const struct tm *restrict)))
		pc_fail("wcsftime() is not declared as size_t wcsftime(wchar_t *restrict, size_t, const wchar_t *restrict, const struct tm *restrict)");
	if (IS_T(&(wcsftime), size_t (*)(wchar_t *restrict, size_t, const wchar_t *restrict, const struct tm *restrict, int)))
		pc_fail("wcsftime() is declared without a prototype");
	if (!IS_T(&(wcslen), size_t (*)(const wchar_t *)))
		pc_fail("wcslen() is not declared as size_t wcslen(const wchar_t *)");
	if (IS_T(&(wcslen), size_t (*)(const wchar_t *, int)))
		pc_fail("wcslen() is declared without a prototype");
	if (!IS_T(&(wcsncasecmp), int (*)(const wchar_t *, const wchar_t *, size_t)))
		pc_fail("wcsncasecmp() is not declared as int wcsncasecmp(const wchar_t *, const wchar_t *, size_t)");
	if (IS_T(&(wcsncasecmp), int (*)(const wchar_t *, const wchar_t *, size_t, int)))
		pc_fail("wcsncasecmp() is declared without a prototype");
	if (!IS_T(&(wcsncasecmp_l), int (*)(const wchar_t *, const wchar_t *, size_t, locale_t)))
		pc_fail("wcsncasecmp_l() is not declared as int wcsncasecmp_l(const wchar_t *, const wchar_t *, size_t, locale_t)");
	if (IS_T(&(wcsncasecmp_l), int (*)(const wchar_t *, const wchar_t *, size_t, locale_t, int)))
		pc_fail("wcsncasecmp_l() is declared without a prototype");
	if (!IS_T(&(wcsncat), wchar_t *(*)(wchar_t *restrict, const wchar_t *restrict, size_t)))
		pc_fail("wcsncat() is not declared as wchar_t *wcsncat(wchar_t *restrict, const wchar_t *restrict, size_t)");
	if (IS_T(&(wcsncat), wchar_t *(*)(wchar_t *restrict, const wchar_t *restrict, size_t, int)))
		pc_fail("wcsncat() is declared without a prototype");
	if (!IS_T(&(wcsncmp), int (*)(const wchar_t *, const wchar_t *, size_t)))
		pc_fail("wcsncmp() is not declared as int wcsncmp(const wchar_t *, const wchar_t *, size_t)");
	if (IS_T(&(wcsncmp), int (*)(const wchar_t *, const wchar_t *, size_t, int)))
		pc_fail("wcsncmp() is declared without a prototype");
	if (!IS_T(&(wcsncpy), wchar_t *(*)(wchar_t *restrict, const wchar_t *restrict, size_t)))
		pc_fail("wcsncpy() is not declared as wchar_t *wcsncpy(wchar_t *restrict, const wchar_t *restrict, size_t)");
	if (IS_T(&(wcsncpy), wchar_t *(*)(wchar_t *restrict, const wchar_t *restrict, size_t, int)))
		pc_fail("wcsncpy() is declared without a prototype");
	if (!IS_T(&(wcsnlen), size_t (*)(const wchar_t *, size_t)))
		pc_fail("wcsnlen() is not declared as size_t wcsnlen(const wchar_t *, size_t)");
	if (IS_T(&(wcsnlen), size_t (*)(const wchar_t *, size_t, int)))
		pc_fail("wcsnlen() is declared without a prototype");
	if (!IS_T(&(wcsnrtombs), size_t (*)(char *restrict, const wchar_t **restrict, size_t, size_t, mbstate_t *restrict)))
		pc_fail("wcsnrtombs() is not declared as size_t wcsnrtombs(char *restrict, const wchar_t **restrict, size_t, size_t, mbstate_t *restrict)");
	if (IS_T(&(wcsnrtombs), size_t (*)(char *restrict, const wchar_t **restrict, size_t, size_t, mbstate_t *restrict, int)))
		pc_fail("wcsnrtombs() is declared without a prototype");
	if (!IS_T(&(wcspbrk), wchar_t *(*)(const wchar_t *, const wchar_t *)))
		pc_fail("wcspbrk() is not declared as wchar_t *wcspbrk(const wchar_t *, const wchar_t *)");
	if (IS_T(&(wcspbrk), wchar_t *(*)(const wchar_t *, const wchar_t *, int)))
		pc_fail("wcspbrk() is declared without a prototype");
	if (!IS_T(&(wcsrchr), wchar_t *(*)(const wchar_t *, wchar_t)))
		pc_fail("wcsrchr() is not declared as wchar_t *wcsrchr(const wchar_t *, wchar_t)");
	if (IS_T(&(wcsrchr), wchar_t *(*)(const wchar_t *, wchar_t, int)))
		pc_fail("wcsrchr() is declared without a prototype");
	if (!IS_T(&(wcsrtombs), size_t (*)(char *restrict, const wchar_t **restrict, size_t, mbstate_t *restrict)))
		pc_fail("wcsrtombs() is not declared as size_t wcsrtombs(char *restrict, const wchar_t **restrict, size_t, mbstate_t *restrict)");
	if (IS_T(&(wcsrtombs), size_t (*)(char *restrict, const wchar_t **restrict, size_t, mbstate_t *restrict, int)))
		pc_fail("wcsrtombs() is declared without a prototype");
	if (!IS_T(&(wcsspn), size_t (*)(const wchar_t *, const wchar_t *)))
		pc_fail("wcsspn() is not declared as size_t wcsspn(const wchar_t *, const wchar_t *)");
	if (IS_T(&(wcsspn), size_t (*)(const wchar_t *, const wchar_t *, int)))
		pc_fail("wcsspn() is declared without a prototype");
	if (!IS_T(&(wcsstr), wchar_t *(*)(const wchar_t *restrict, const wchar_t *restrict)))
		pc_fail("wcsstr() is not declared as wchar_t *wcsstr(const wchar_t *restrict, const wchar_t *restrict)");
	if (IS_T(&(wcsstr), wchar_t *(*)(const wchar_t *restrict, const wchar_t *restrict, int)))
		pc_fail("wcsstr() is declared without a prototype");
	if (!IS_T(&(wcstod), double (*)(const wchar_t *restrict, wchar_t **restrict)))
		pc_fail("wcstod() is not declared as double wcstod(const wchar_t *restrict, wchar_t **restrict)");
	if (IS_T(&(wcstod), double (*)(const wchar_t *restrict, wchar_t **restrict, int)))
		pc_fail("wcstod() is declared without a prototype");
	if (!IS_T(&(wcstof), float (*)(const wchar_t *restrict, wchar_t **restrict)))
		pc_fail("wcstof() is not declared as float wcstof(const wchar_t *restrict, wchar_t **restrict)");
	if (IS_T(&(wcstof), float (*)(const wchar_t *restrict, wchar_t **restrict, int)))
		pc_fail("wcstof() is declared without a prototype");
	if (!IS_T(&(wcstok), wchar_t *(*)(wchar_t *restrict, const wchar_t *restrict, wchar_t **restrict)))
		pc_fail("wcstok() is not declared as wchar_t *wcstok(wchar_t *restrict, const wchar_t *restrict, wchar_t **restrict)");
	if (IS_T(&(wcstok), wchar_t *(*)(wchar_t *restrict, const wchar_t *restrict, wchar_t **restrict, int)))
		pc_fail("wcstok() is declared without a prototype");
	if (!IS_T(&(wcstol), long (*)(const wchar_t *restrict, wchar_t **restrict, int)))
		pc_fail("wcstol() is not declared as long wcstol(const wchar_t *restrict, wchar_t **restrict, int)");
	if (IS_T(&(wcstol), long (*)(const wchar_t *restrict, wchar_t **restrict, int, int)))
		pc_fail("wcstol() is declared without a prototype");
	if (!IS_T(&(wcstold), long double (*)(const wchar_t *restrict, wchar_t **restrict)))
		pc_fail("wcstold() is not declared as long double wcstold(const wchar_t *restrict, wchar_t **restrict)");
	if (IS_T(&(wcstold), long double (*)(const wchar_t *restrict, wchar_t **restrict, int)))
		pc_fail("wcstold() is declared without a prototype");
	if (!IS_T(&(wcstoll), long long (*)(const wchar_t *restrict, wchar_t **restrict, int)))
		pc_fail("wcstoll() is not declared as long long wcstoll(const wchar_t *restrict, wchar_t **restrict, int)");
	if (IS_T(&(wcstoll), long long (*)(const wchar_t *restrict, wchar_t **restrict, int, int)))
		pc_fail("wcstoll() is declared without a prototype");
	if (!IS_T(&(wcstoul), unsigned long (*)(const wchar_t *restrict, wchar_t **restrict, int)))
		pc_fail("wcstoul() is not declared as unsigned long wcstoul(const wchar_t *restrict, wchar_t **restrict, int)");
	if (IS_T(&(wcstoul), unsigned long (*)(const wchar_t *restrict, wchar_t **restrict, int, int)))
		pc_fail("wcstoul() is declared without a prototype");
	if (!IS_T(&(wcstoull), unsigned long long (*)(const wchar_t *restrict, wchar_t **restrict, int)))
		pc_fail("wcstoull() is not declared as unsigned long long wcstoull(const wchar_t *restrict, wchar_t **restrict, int)");
	if (IS_T(&(wcstoull), unsigned long long (*)(const wchar_t *restrict, wchar_t **restrict, int, int)))
		pc_fail("wcstoull() is declared without a prototype");
	if (!IS_T(&(wcsxfrm), size_t (*)(wchar_t *restrict, const wchar_t *restrict, size_t)))
		pc_fail("wcsxfrm() is not declared as size_t wcsxfrm(wchar_t *restrict, const wchar_t *restrict, size_t)");
	if (IS_T(&(wcsxfrm), size_t (*)(wchar_t *restrict, const wchar_t *restrict, size_t, int)))
		pc_fail("wcsxfrm() is declared without a prototype");
	if (!IS_T(&(wcsxfrm_l), size_t (*)(wchar_t *restrict, const wchar_t *restrict, size_t, locale_t)))
		pc_fail("wcsxfrm_l() is not declared as size_t wcsxfrm_l(wchar_t *restrict, const wchar_t *restrict, size_t, locale_t)");
	if (IS_T(&(wcsxfrm_l), size_t (*)(wchar_t *restrict, const wchar_t *restrict, size_t, locale_t, int)))
		pc_fail("wcsxfrm_l() is declared without a prototype");
	if (!IS_T(&(wctob), int (*)(wint_t)))
		pc_fail("wctob() is not declared as int wctob(wint_t)");
	if (IS_T(&(wctob), int (*)(wint_t, int)))
		pc_fail("wctob() is declared without a prototype");
	if (!IS_T(&(wmemchr), wchar_t *(*)(const wchar_t *, wchar_t, size_t)))
		pc_fail("wmemchr() is not declared as wchar_t *wmemchr(const wchar_t *, wchar_t, size_t)");
	if (IS_T(&(wmemchr), wchar_t *(*)(const wchar_t *, wchar_t, size_t, int)))
		pc_fail("wmemchr() is declared without a prototype");
	if (!IS_T(&(wmemcmp), int (*)(const wchar_t *, const wchar_t *, size_t)))
		pc_fail("wmemcmp() is not declared as int wmemcmp(const wchar_t *, const wchar_t *, size_t)");
	if (IS_T(&(wmemcmp), int (*)(const wchar_t *, const wchar_t *, size_t, int)))
		pc_fail("wmemcmp() is declared without a prototype");
	if (!IS_T(&(wmemcpy), wchar_t *(*)(wchar_t *restrict, const wchar_t *restrict, size_t)))
		pc_fail("wmemcpy() is not declared as wchar_t *wmemcpy(wchar_t *restrict, const wchar_t *restrict, size_t)");
	if (IS_T(&(wmemcpy), wchar_t *(*)(wchar_t *restrict, const wchar_t *restrict, size_t, int)))
		pc_fail("wmemcpy() is declared without a prototype");
	if (!IS_T(&(wmemmove), wchar_t *(*)(wchar_t *, const wchar_t *, size_t)))
		pc_fail("wmemmove() is not declared as wchar_t *wmemmove(wchar_t *, const wchar_t *, size_t)");
	if (IS_T(&(wmemmove), wchar_t *(*)(wchar_t *, const wchar_t *, size_t, int)))
		pc_fail("wmemmove() is declared without a prototype");
	if (!IS_T(&(wmemset), wchar_t *(*)(wchar_t *, wchar_t, size_t)))
		pc_fail("wmemset() is not declared as wchar_t *wmemset(wchar_t *, wchar_t, size_t)");
	if (IS_T(&(wmemset), wchar_t *(*)(wchar_t *, wchar_t, size_t, int)))
		pc_fail("wmemset() is declared without a prototype");
	if (!IS_T(&(wprintf), int (*)(const wchar_t *restrict, ...)))
		pc_fail("wprintf() is not declared as int wprintf(const wchar_t *restrict, ...)");
	if (!IS_T(&(wscanf), int (*)(const wchar_t *restrict, ...)))
		pc_fail("wscanf() is not declared as int wscanf(const wchar_t *restrict, ...)");
	pc_pass();
}
