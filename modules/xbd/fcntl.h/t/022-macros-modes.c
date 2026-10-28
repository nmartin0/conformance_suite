/*
 * <fcntl.h> macros modes
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.022
 *
 * Only <fcntl.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <fcntl.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.fcntl.h.022");
	pc_test();
#ifndef S_IRWXU
	pc_fail("<fcntl.h> does not define S_IRWXU");
#endif
#ifndef S_IRUSR
	pc_fail("<fcntl.h> does not define S_IRUSR");
#endif
#ifndef S_IWUSR
	pc_fail("<fcntl.h> does not define S_IWUSR");
#endif
#ifndef S_IXUSR
	pc_fail("<fcntl.h> does not define S_IXUSR");
#endif
#ifndef S_IRWXG
	pc_fail("<fcntl.h> does not define S_IRWXG");
#endif
#ifndef S_IRGRP
	pc_fail("<fcntl.h> does not define S_IRGRP");
#endif
#ifndef S_IWGRP
	pc_fail("<fcntl.h> does not define S_IWGRP");
#endif
#ifndef S_IXGRP
	pc_fail("<fcntl.h> does not define S_IXGRP");
#endif
#ifndef S_IRWXO
	pc_fail("<fcntl.h> does not define S_IRWXO");
#endif
#ifndef S_IROTH
	pc_fail("<fcntl.h> does not define S_IROTH");
#endif
#ifndef S_IWOTH
	pc_fail("<fcntl.h> does not define S_IWOTH");
#endif
#ifndef S_IXOTH
	pc_fail("<fcntl.h> does not define S_IXOTH");
#endif
#ifndef S_ISUID
	pc_fail("<fcntl.h> does not define S_ISUID");
#endif
#ifndef S_ISGID
	pc_fail("<fcntl.h> does not define S_ISGID");
#endif
	pc_pass();
}
