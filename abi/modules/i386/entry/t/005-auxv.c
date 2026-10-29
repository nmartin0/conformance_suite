/*
 * The auxiliary vector follows the environment pointers, ends with a null entry, and the entries the
 * process can verify hold the right values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.entry.005
 */
#include "../../../../check.h"
#include <unistd.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.entry.005")
#else
enum { AT_NULL_ = 0, AT_PHENT_ = 4, AT_PHNUM_ = 5, AT_PAGESZ_ = 6, AT_NOTELF_ = 10, AT_UID_ = 11,
    AT_EUID_ = 12, AT_GID_ = 13, AT_EGID_ = 14, AT_CLKTCK_ = 17, AT_SECURE_ = 23 };

int main(int argc, char **argv, char **envp)
{
	const unsigned *a;
	int i, ended = 0;
	long v;

	pc_begin("ABI.i386.entry.005");
	pc_test();
	(void)argc; (void)argv;
	/* Inferred layout: pairs of 32-bit words start right after the null that ends the environment. */
	for (i = 0; envp[i] != NULL; i++)
		pc_check(i < 1000000, "environment array not terminated");
	a = (const unsigned *)(envp + i + 1);
	for (i = 0; i < 4096; i += 2) {
		unsigned t = a[i], val = a[i + 1];

		if (t == AT_NULL_) {
			ended = 1;
			break;
		}
		switch (t) {
		case AT_PHENT_:
			pc_check(val == 32, "AT_PHENT is %u, the size of a 32-bit program header is 32", val);
			break;
		case AT_PHNUM_:
			pc_check(val > 0, "AT_PHNUM is 0");
			break;
		case AT_PAGESZ_:
			v = sysconf(_SC_PAGESIZE);
			pc_check(v == -1 || (unsigned long)v == val, "AT_PAGESZ is %u, sysconf says %ld", val, v);
			break;
		case AT_NOTELF_:
			pc_check(val == 0, "AT_NOTELF is %u for an ELF program", val);
			break;
		case AT_UID_:
			pc_check(val == (unsigned)getuid(), "AT_UID is %u, getuid() says %u", val, (unsigned)getuid());
			break;
		case AT_EUID_:
			pc_check(val == (unsigned)geteuid(), "AT_EUID is %u, geteuid() says %u", val, (unsigned)geteuid());
			break;
		case AT_GID_:
			pc_check(val == (unsigned)getgid(), "AT_GID is %u, getgid() says %u", val, (unsigned)getgid());
			break;
		case AT_EGID_:
			pc_check(val == (unsigned)getegid(), "AT_EGID is %u, getegid() says %u", val, (unsigned)getegid());
			break;
		case AT_CLKTCK_:
			v = sysconf(_SC_CLK_TCK);
			pc_check(v == -1 || (unsigned long)v == val, "AT_CLKTCK is %u, sysconf says %ld", val, v);
			break;
		case AT_SECURE_:
			pc_check(val <= 1, "AT_SECURE is %u, not 0 or 1", val);
			break;
		default:
			break;
		}
	}
	pc_check(ended, "no AT_NULL entry within 2048 entries of the auxiliary vector");
	pc_pass();
}
#endif
