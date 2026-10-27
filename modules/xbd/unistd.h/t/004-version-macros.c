/*
 * _POSIX_VERSION and _POSIX2_VERSION values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.004
 */
#include <unistd.h>
#include "pc.h"

#if !defined(_POSIX_VERSION) || _POSIX_VERSION != 202405L
#error "_POSIX_VERSION is not defined as 202405L"
#endif
#ifdef _POSIX_SUBPROFILE
#if defined(_POSIX2_VERSION) && _POSIX2_VERSION != 202405L && _POSIX2_VERSION != -1
#error "_POSIX2_VERSION is neither 202405L nor -1 on a subprofile"
#endif
#else
#if !defined(_POSIX2_VERSION) || _POSIX2_VERSION != 202405L
#error "_POSIX2_VERSION is not defined as 202405L"
#endif
#endif

int main(void)
{
	pc_begin("XBD.unistd.h.004");
	pc_test();
#ifdef _POSIX_SUBPROFILE
#if !defined(_POSIX2_VERSION) || _POSIX2_VERSION == -1
	{
		long r = sysconf(_SC_2_VERSION);

		pc_check(r == 202405L || r == -1, "sysconf(_SC_2_VERSION) is %ld, neither 202405L nor -1", r);
	}
#endif
#endif
	pc_pass();
}
