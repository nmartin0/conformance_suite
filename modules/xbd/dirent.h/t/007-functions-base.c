/*
 * <dirent.h> functions base
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.dirent.h.007
 *
 * Only <dirent.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <dirent.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.dirent.h.007");
	pc_test();
	if (!IS_T(&(alphasort), int (*)(const struct dirent **, const struct dirent **)))
		pc_fail("alphasort() is not declared as int alphasort(const struct dirent **, const struct dirent **)");
	if (IS_T(&(alphasort), int (*)(const struct dirent **, const struct dirent **, int)))
		pc_fail("alphasort() is declared without a prototype");
	if (!IS_T(&(closedir), int (*)(DIR *)))
		pc_fail("closedir() is not declared as int closedir(DIR *)");
	if (IS_T(&(closedir), int (*)(DIR *, int)))
		pc_fail("closedir() is declared without a prototype");
	if (!IS_T(&(dirfd), int (*)(DIR *)))
		pc_fail("dirfd() is not declared as int dirfd(DIR *)");
	if (IS_T(&(dirfd), int (*)(DIR *, int)))
		pc_fail("dirfd() is declared without a prototype");
	if (!IS_T(&(fdopendir), DIR *(*)(int)))
		pc_fail("fdopendir() is not declared as DIR *fdopendir(int)");
	if (IS_T(&(fdopendir), DIR *(*)(int, int)))
		pc_fail("fdopendir() is declared without a prototype");
	if (!IS_T(&(opendir), DIR *(*)(const char *)))
		pc_fail("opendir() is not declared as DIR *opendir(const char *)");
	if (IS_T(&(opendir), DIR *(*)(const char *, int)))
		pc_fail("opendir() is declared without a prototype");
	if (!IS_T(&(readdir), struct dirent *(*)(DIR *)))
		pc_fail("readdir() is not declared as struct dirent *readdir(DIR *)");
	if (IS_T(&(readdir), struct dirent *(*)(DIR *, int)))
		pc_fail("readdir() is declared without a prototype");
	if (!IS_T(&(readdir_r), int (*)(DIR *restrict, struct dirent *restrict, struct dirent **restrict)))
		pc_fail("readdir_r() is not declared as int readdir_r(DIR *restrict, struct dirent *restrict, struct dirent **restrict)");
	if (IS_T(&(readdir_r), int (*)(DIR *restrict, struct dirent *restrict, struct dirent **restrict, int)))
		pc_fail("readdir_r() is declared without a prototype");
	if (!IS_T(&(rewinddir), void (*)(DIR *)))
		pc_fail("rewinddir() is not declared as void rewinddir(DIR *)");
	if (IS_T(&(rewinddir), void (*)(DIR *, int)))
		pc_fail("rewinddir() is declared without a prototype");
	if (!IS_T(&(scandir), int (*)(const char *, struct dirent ***, int (*)(const struct dirent *), int (*)(const struct dirent **, const struct dirent **))))
		pc_fail("scandir() is not declared as int scandir(const char *, struct dirent ***, int (*)(const struct dirent *), int (*)(const struct dirent **, const struct dirent **))");
	if (IS_T(&(scandir), int (*)(const char *, struct dirent ***, int (*)(const struct dirent *), int (*)(const struct dirent **, const struct dirent **), int)))
		pc_fail("scandir() is declared without a prototype");
	pc_pass();
}
