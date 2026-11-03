/*
 * <string.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.string.h.003
 *
 * Only <string.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <string.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.string.h.003");
	pc_test();
	if (!IS_T(&(memcmp), int (*)(const void *, const void *, size_t)))
		pc_fail("memcmp() is not declared as int memcmp(const void *, const void *, size_t)");
	if (IS_T(&(memcmp), int (*)(const void *, const void *, size_t, int)))
		pc_fail("memcmp() is declared without a prototype");
	if (!IS_T(&(memchr), void *(*)(const void *, int, size_t)))
		pc_fail("memchr() is not declared as void *memchr(const void *, int, size_t)");
	if (IS_T(&(memchr), void *(*)(const void *, int, size_t, int)))
		pc_fail("memchr() is declared without a prototype");
	if (!IS_T(&(memcpy), void *(*)(void *, const void *, size_t)))
		pc_fail("memcpy() is not declared as void *memcpy(void *restrict, const void *restrict, size_t)");
	if (IS_T(&(memcpy), void *(*)(void *, const void *, size_t, int)))
		pc_fail("memcpy() is declared without a prototype");
	if (!IS_T(&(memmove), void *(*)(void *, const void *, size_t)))
		pc_fail("memmove() is not declared as void *memmove(void *, const void *, size_t)");
	if (IS_T(&(memmove), void *(*)(void *, const void *, size_t, int)))
		pc_fail("memmove() is declared without a prototype");
	if (!IS_T(&(memset), void *(*)(void *, int, size_t)))
		pc_fail("memset() is not declared as void *memset(void *, int, size_t)");
	if (IS_T(&(memset), void *(*)(void *, int, size_t, int)))
		pc_fail("memset() is declared without a prototype");
	if (!IS_T(&(strcat), char *(*)(char *, const char *)))
		pc_fail("strcat() is not declared as char *strcat(char *restrict, const char *restrict)");
	if (IS_T(&(strcat), char *(*)(char *, const char *, int)))
		pc_fail("strcat() is declared without a prototype");
	if (!IS_T(&(strchr), char *(*)(const char *, int)))
		pc_fail("strchr() is not declared as char *strchr(const char *, int)");
	if (IS_T(&(strchr), char *(*)(const char *, int, int)))
		pc_fail("strchr() is declared without a prototype");
	if (!IS_T(&(strcmp), int (*)(const char *, const char *)))
		pc_fail("strcmp() is not declared as int strcmp(const char *, const char *)");
	if (IS_T(&(strcmp), int (*)(const char *, const char *, int)))
		pc_fail("strcmp() is declared without a prototype");
	if (!IS_T(&(strcoll), int (*)(const char *, const char *)))
		pc_fail("strcoll() is not declared as int strcoll(const char *, const char *)");
	if (IS_T(&(strcoll), int (*)(const char *, const char *, int)))
		pc_fail("strcoll() is declared without a prototype");
	if (!IS_T(&(strcpy), char *(*)(char *, const char *)))
		pc_fail("strcpy() is not declared as char *strcpy(char *restrict, const char *restrict)");
	if (IS_T(&(strcpy), char *(*)(char *, const char *, int)))
		pc_fail("strcpy() is declared without a prototype");
	if (!IS_T(&(strcspn), size_t (*)(const char *, const char *)))
		pc_fail("strcspn() is not declared as size_t strcspn(const char *, const char *)");
	if (IS_T(&(strcspn), size_t (*)(const char *, const char *, int)))
		pc_fail("strcspn() is declared without a prototype");
	if (!IS_T(&(strerror), char *(*)(int)))
		pc_fail("strerror() is not declared as char *strerror(int)");
	if (IS_T(&(strerror), char *(*)(int, int)))
		pc_fail("strerror() is declared without a prototype");
	if (!IS_T(&(strerror_r), int (*)(int, char *, size_t)))
		pc_fail("strerror_r() is not declared as int strerror_r(int, char *, size_t)");
	if (IS_T(&(strerror_r), int (*)(int, char *, size_t, int)))
		pc_fail("strerror_r() is declared without a prototype");
	if (!IS_T(&(strlcat), size_t (*)(char *, const char *, size_t)))
		pc_fail("strlcat() is not declared as size_t strlcat(char *restrict, const char *restrict, size_t)");
	if (IS_T(&(strlcat), size_t (*)(char *, const char *, size_t, int)))
		pc_fail("strlcat() is declared without a prototype");
	if (!IS_T(&(strlcpy), size_t (*)(char *, const char *, size_t)))
		pc_fail("strlcpy() is not declared as size_t strlcpy(char *restrict, const char *restrict, size_t)");
	if (IS_T(&(strlcpy), size_t (*)(char *, const char *, size_t, int)))
		pc_fail("strlcpy() is declared without a prototype");
	if (!IS_T(&(strlen), size_t (*)(const char *)))
		pc_fail("strlen() is not declared as size_t strlen(const char *)");
	if (IS_T(&(strlen), size_t (*)(const char *, int)))
		pc_fail("strlen() is declared without a prototype");
	if (!IS_T(&(strncat), char *(*)(char *, const char *, size_t)))
		pc_fail("strncat() is not declared as char *strncat(char *restrict, const char *restrict, size_t)");
	if (IS_T(&(strncat), char *(*)(char *, const char *, size_t, int)))
		pc_fail("strncat() is declared without a prototype");
	if (!IS_T(&(strncmp), int (*)(const char *, const char *, size_t)))
		pc_fail("strncmp() is not declared as int strncmp(const char *, const char *, size_t)");
	if (IS_T(&(strncmp), int (*)(const char *, const char *, size_t, int)))
		pc_fail("strncmp() is declared without a prototype");
	if (!IS_T(&(strncpy), char *(*)(char *, const char *, size_t)))
		pc_fail("strncpy() is not declared as char *strncpy(char *restrict, const char *restrict, size_t)");
	if (IS_T(&(strncpy), char *(*)(char *, const char *, size_t, int)))
		pc_fail("strncpy() is declared without a prototype");
	if (!IS_T(&(strnlen), size_t (*)(const char *, size_t)))
		pc_fail("strnlen() is not declared as size_t strnlen(const char *, size_t)");
	if (IS_T(&(strnlen), size_t (*)(const char *, size_t, int)))
		pc_fail("strnlen() is declared without a prototype");
	if (!IS_T(&(strpbrk), char *(*)(const char *, const char *)))
		pc_fail("strpbrk() is not declared as char *strpbrk(const char *, const char *)");
	if (IS_T(&(strpbrk), char *(*)(const char *, const char *, int)))
		pc_fail("strpbrk() is declared without a prototype");
	if (!IS_T(&(strrchr), char *(*)(const char *, int)))
		pc_fail("strrchr() is not declared as char *strrchr(const char *, int)");
	if (IS_T(&(strrchr), char *(*)(const char *, int, int)))
		pc_fail("strrchr() is declared without a prototype");
	if (!IS_T(&(strspn), size_t (*)(const char *, const char *)))
		pc_fail("strspn() is not declared as size_t strspn(const char *, const char *)");
	if (IS_T(&(strspn), size_t (*)(const char *, const char *, int)))
		pc_fail("strspn() is declared without a prototype");
	if (!IS_T(&(strstr), char *(*)(const char *, const char *)))
		pc_fail("strstr() is not declared as char *strstr(const char *, const char *)");
	if (IS_T(&(strstr), char *(*)(const char *, const char *, int)))
		pc_fail("strstr() is declared without a prototype");
	if (!IS_T(&(strtok), char *(*)(char *, const char *)))
		pc_fail("strtok() is not declared as char *strtok(char *restrict, const char *restrict)");
	if (IS_T(&(strtok), char *(*)(char *, const char *, int)))
		pc_fail("strtok() is declared without a prototype");
	if (!IS_T(&(strxfrm), size_t (*)(char *, const char *, size_t)))
		pc_fail("strxfrm() is not declared as size_t strxfrm(char *restrict, const char *restrict, size_t)");
	if (IS_T(&(strxfrm), size_t (*)(char *, const char *, size_t, int)))
		pc_fail("strxfrm() is declared without a prototype");
	if (!IS_T(&(memmem), void *(*)(const void *, size_t, const void *, size_t)))
		pc_fail("memmem() is not declared as void *memmem(const void *, size_t, const void *, size_t)");
	if (IS_T(&(memmem), void *(*)(const void *, size_t, const void *, size_t, int)))
		pc_fail("memmem() is declared without a prototype");
	if (!IS_T(&(stpcpy), char *(*)(char *, const char *)))
		pc_fail("stpcpy() is not declared as char *stpcpy(char *restrict, const char *restrict)");
	if (IS_T(&(stpcpy), char *(*)(char *, const char *, int)))
		pc_fail("stpcpy() is declared without a prototype");
	if (!IS_T(&(stpncpy), char *(*)(char *, const char *, size_t)))
		pc_fail("stpncpy() is not declared as char *stpncpy(char *restrict, const char *restrict, size_t)");
	if (IS_T(&(stpncpy), char *(*)(char *, const char *, size_t, int)))
		pc_fail("stpncpy() is declared without a prototype");
	if (!IS_T(&(strcoll_l), int (*)(const char *, const char *, locale_t)))
		pc_fail("strcoll_l() is not declared as int strcoll_l(const char *, const char *, locale_t)");
	if (IS_T(&(strcoll_l), int (*)(const char *, const char *, locale_t, int)))
		pc_fail("strcoll_l() is declared without a prototype");
	if (!IS_T(&(strdup), char *(*)(const char *)))
		pc_fail("strdup() is not declared as char *strdup(const char *)");
	if (IS_T(&(strdup), char *(*)(const char *, int)))
		pc_fail("strdup() is declared without a prototype");
	if (!IS_T(&(strerror_l), char *(*)(int, locale_t)))
		pc_fail("strerror_l() is not declared as char *strerror_l(int, locale_t)");
	if (IS_T(&(strerror_l), char *(*)(int, locale_t, int)))
		pc_fail("strerror_l() is declared without a prototype");
	if (!IS_T(&(strndup), char *(*)(const char *, size_t)))
		pc_fail("strndup() is not declared as char *strndup(const char *, size_t)");
	if (IS_T(&(strndup), char *(*)(const char *, size_t, int)))
		pc_fail("strndup() is declared without a prototype");
	if (!IS_T(&(strsignal), char *(*)(int)))
		pc_fail("strsignal() is not declared as char *strsignal(int)");
	if (IS_T(&(strsignal), char *(*)(int, int)))
		pc_fail("strsignal() is declared without a prototype");
	if (!IS_T(&(strtok_r), char *(*)(char *, const char *, char **)))
		pc_fail("strtok_r() is not declared as char *strtok_r(char *restrict, const char *restrict, char **restrict)");
	if (IS_T(&(strtok_r), char *(*)(char *, const char *, char **, int)))
		pc_fail("strtok_r() is declared without a prototype");
	if (!IS_T(&(strxfrm_l), size_t (*)(char *, const char *, size_t, locale_t)))
		pc_fail("strxfrm_l() is not declared as size_t strxfrm_l(char *restrict, const char *restrict, size_t, locale_t)");
	if (IS_T(&(strxfrm_l), size_t (*)(char *, const char *, size_t, locale_t, int)))
		pc_fail("strxfrm_l() is declared without a prototype");
	pc_pass();
}
