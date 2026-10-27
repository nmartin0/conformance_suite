/*
 * S_IRWXU, S_IRUSR, S_IWUSR, S_IXUSR, S_IRWXG, S_IRGRP, S_IWGRP, S_IXGRP,
 * S_IRWXO, S_IROTH, S_IWOTH and S_IXOTH are usable in #if and have the
 * octal values 0700, 0400, 0200, 0100, 070, 040, 020, 010, 07, 04, 02 and
 * 01.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_stat.h.001
 */
#include "pc.h"
#include <sys/stat.h>

int main(void)
{
	pc_begin("XBD.sys_stat.h.001");
#if !defined(S_IRWXU) || !defined(S_IRUSR) || !defined(S_IWUSR) || !defined(S_IXUSR) || !defined(S_IRWXG) || !defined(S_IRGRP) || !defined(S_IWGRP) || !defined(S_IXGRP) || !defined(S_IRWXO) || !defined(S_IROTH) || !defined(S_IWOTH) || !defined(S_IXOTH)
	pc_unresolved("a constant is missing (the header check reports it): S_IRWXU S_IRUSR S_IWUSR S_IXUSR S_IRWXG S_IRGRP S_IWGRP S_IXGRP S_IRWXO S_IROTH S_IWOTH S_IXOTH");
#else
	pc_test();
#if S_IRWXU != 0700
	pc_fail("S_IRWXU has the wrong value");
#elif S_IRUSR != 0400
	pc_fail("S_IRUSR has the wrong value");
#elif S_IWUSR != 0200
	pc_fail("S_IWUSR has the wrong value");
#elif S_IXUSR != 0100
	pc_fail("S_IXUSR has the wrong value");
#elif S_IRWXG != 070
	pc_fail("S_IRWXG has the wrong value");
#elif S_IRGRP != 040
	pc_fail("S_IRGRP has the wrong value");
#elif S_IWGRP != 020
	pc_fail("S_IWGRP has the wrong value");
#elif S_IXGRP != 010
	pc_fail("S_IXGRP has the wrong value");
#elif S_IRWXO != 07
	pc_fail("S_IRWXO has the wrong value");
#elif S_IROTH != 04
	pc_fail("S_IROTH has the wrong value");
#elif S_IWOTH != 02
	pc_fail("S_IWOTH has the wrong value");
#elif S_IXOTH != 01
	pc_fail("S_IXOTH has the wrong value");
#else
	pc_pass();
#endif
#endif
}
