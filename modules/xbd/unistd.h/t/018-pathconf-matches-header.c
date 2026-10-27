/*
 * pathconf() agrees with the execution-time symbols.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.018
 */
#include <unistd.h>
#include "pc.h"

int main(void)
{
	long r;

	pc_begin("XBD.unistd.h.018");
	pc_make_file("f", "x");
	pc_test();
#ifdef _POSIX_ASYNC_IO
#if _POSIX_ASYNC_IO == -1
	r = pathconf("f", _PC_ASYNC_IO);
	pc_check(r == -1, "_POSIX_ASYNC_IO is -1 but pathconf(_PC_ASYNC_IO) is %ld", r);
#else
	r = pathconf("f", _PC_ASYNC_IO);
	pc_check(r != -1, "_POSIX_ASYNC_IO is not -1 but pathconf(_PC_ASYNC_IO) is -1");
#endif
#endif
#ifdef _POSIX_FALLOC
#if _POSIX_FALLOC == -1
	r = pathconf("f", _PC_FALLOC);
	pc_check(r == -1, "_POSIX_FALLOC is -1 but pathconf(_PC_FALLOC) is %ld", r);
#else
	r = pathconf("f", _PC_FALLOC);
	pc_check(r != -1, "_POSIX_FALLOC is not -1 but pathconf(_PC_FALLOC) is -1");
#endif
#endif
#ifdef _POSIX_PRIO_IO
#if _POSIX_PRIO_IO == -1
	r = pathconf("f", _PC_PRIO_IO);
	pc_check(r == -1, "_POSIX_PRIO_IO is -1 but pathconf(_PC_PRIO_IO) is %ld", r);
#else
	r = pathconf("f", _PC_PRIO_IO);
	pc_check(r != -1, "_POSIX_PRIO_IO is not -1 but pathconf(_PC_PRIO_IO) is -1");
#endif
#endif
#ifdef _POSIX_SYNC_IO
#if _POSIX_SYNC_IO == -1
	r = pathconf("f", _PC_SYNC_IO);
	pc_check(r == -1, "_POSIX_SYNC_IO is -1 but pathconf(_PC_SYNC_IO) is %ld", r);
#else
	r = pathconf("f", _PC_SYNC_IO);
	pc_check(r != -1, "_POSIX_SYNC_IO is not -1 but pathconf(_PC_SYNC_IO) is -1");
#endif
#endif
#ifdef _POSIX_CHOWN_RESTRICTED
#if _POSIX_CHOWN_RESTRICTED == -1
	r = pathconf("f", _PC_CHOWN_RESTRICTED);
	pc_check(r == -1, "_POSIX_CHOWN_RESTRICTED is -1 but pathconf(_PC_CHOWN_RESTRICTED) is %ld", r);
#else
	r = pathconf("f", _PC_CHOWN_RESTRICTED);
	pc_check(r != -1, "_POSIX_CHOWN_RESTRICTED is not -1 but pathconf(_PC_CHOWN_RESTRICTED) is -1");
#endif
#endif
#ifdef _POSIX_NO_TRUNC
#if _POSIX_NO_TRUNC == -1
	r = pathconf(".", _PC_NO_TRUNC);
	pc_check(r == -1, "_POSIX_NO_TRUNC is -1 but pathconf(_PC_NO_TRUNC) is %ld", r);
#else
	r = pathconf(".", _PC_NO_TRUNC);
	pc_check(r != -1, "_POSIX_NO_TRUNC is not -1 but pathconf(_PC_NO_TRUNC) is -1");
#endif
#endif
	pc_pass();
}
