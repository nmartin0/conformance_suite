/*
 * The types the page calls integer types are integers; clock_t is arithmetic.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.11-data-types.003
 */
#include "pc.h"
#include <poll.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <wchar.h>

#define OPS(T) ((T)((((T)1 << 1) | ((T)6 & (T)3)) ^ ~(T)0) >> 1)

/* an integer type truncates a fraction */
#define HALF(T) do { volatile double d = 0.5; volatile T t = (T)d; \
	pc_check(t == 0, "%s: 0.5 converted to the type is not 0", #T); } while (0)

int main(void)
{
	volatile int v;
	volatile double d = 0.5;
	clock_t c;

	pc_begin("XSH.2.11-data-types.003");
	pc_test();
	v = (OPS(dev_t) != 0) + (OPS(gid_t) != 0) + (OPS(uid_t) != 0) + (OPS(id_t) != 0) + (OPS(mode_t) != 0)
	    + (OPS(nlink_t) != 0) + (OPS(nfds_t) != 0) + (OPS(time_t) != 0) + (OPS(wchar_t) != 0) + (OPS(wint_t) != 0)
	    + (OPS(sig_atomic_t) != 0);
	(void)v;
	HALF(dev_t);
	HALF(gid_t);
	HALF(uid_t);
	HALF(id_t);
	HALF(mode_t);
	HALF(nlink_t);
	HALF(nfds_t);
	HALF(time_t);
	HALF(wchar_t);
	HALF(wint_t);
	HALF(sig_atomic_t);
	c = (clock_t)d;
	pc_check(c == 0 || (c > 0 && c < 1), "clock_t: 0.5 converted to the type is neither 0 nor between 0 and 1");
	pc_check(c + 1 > c, "clock_t does not take arithmetic");
	pc_pass();
}
