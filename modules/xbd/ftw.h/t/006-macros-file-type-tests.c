/*
 * <ftw.h> macros file type tests
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.ftw.h.006
 *
 * Only <ftw.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <ftw.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.ftw.h.006");
	pc_test();
#ifndef S_ISBLK
	pc_fail("<ftw.h> does not define S_ISBLK");
#endif
#ifndef S_ISCHR
	pc_fail("<ftw.h> does not define S_ISCHR");
#endif
#ifndef S_ISDIR
	pc_fail("<ftw.h> does not define S_ISDIR");
#endif
#ifndef S_ISFIFO
	pc_fail("<ftw.h> does not define S_ISFIFO");
#endif
#ifndef S_ISREG
	pc_fail("<ftw.h> does not define S_ISREG");
#endif
#ifndef S_ISLNK
	pc_fail("<ftw.h> does not define S_ISLNK");
#endif
#ifndef S_ISSOCK
	pc_fail("<ftw.h> does not define S_ISSOCK");
#endif
	pc_pass();
}
