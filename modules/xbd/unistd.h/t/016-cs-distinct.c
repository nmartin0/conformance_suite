/*
 * _CS_ constants differ.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.016
 */
#include <string.h>
#include <unistd.h>
#include "pc.h"

int main(void)
{
	static const struct { const char *name; long val; } c[] = {
		{ "_CS_PATH", (long)_CS_PATH },
		{ "_CS_POSIX_V7_ILP32_OFF32_CFLAGS", (long)_CS_POSIX_V7_ILP32_OFF32_CFLAGS },
		{ "_CS_POSIX_V7_ILP32_OFF32_LDFLAGS", (long)_CS_POSIX_V7_ILP32_OFF32_LDFLAGS },
		{ "_CS_POSIX_V7_ILP32_OFF32_LIBS", (long)_CS_POSIX_V7_ILP32_OFF32_LIBS },
		{ "_CS_POSIX_V7_ILP32_OFFBIG_CFLAGS", (long)_CS_POSIX_V7_ILP32_OFFBIG_CFLAGS },
		{ "_CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS", (long)_CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS },
		{ "_CS_POSIX_V7_ILP32_OFFBIG_LIBS", (long)_CS_POSIX_V7_ILP32_OFFBIG_LIBS },
		{ "_CS_POSIX_V7_LP64_OFF64_CFLAGS", (long)_CS_POSIX_V7_LP64_OFF64_CFLAGS },
		{ "_CS_POSIX_V7_LP64_OFF64_LDFLAGS", (long)_CS_POSIX_V7_LP64_OFF64_LDFLAGS },
		{ "_CS_POSIX_V7_LP64_OFF64_LIBS", (long)_CS_POSIX_V7_LP64_OFF64_LIBS },
		{ "_CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS", (long)_CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS },
		{ "_CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS", (long)_CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS },
		{ "_CS_POSIX_V7_LPBIG_OFFBIG_LIBS", (long)_CS_POSIX_V7_LPBIG_OFFBIG_LIBS },
		{ "_CS_POSIX_V7_THREADS_CFLAGS", (long)_CS_POSIX_V7_THREADS_CFLAGS },
		{ "_CS_POSIX_V7_THREADS_LDFLAGS", (long)_CS_POSIX_V7_THREADS_LDFLAGS },
		{ "_CS_POSIX_V7_WIDTH_RESTRICTED_ENVS", (long)_CS_POSIX_V7_WIDTH_RESTRICTED_ENVS },
		{ "_CS_V7_ENV", (long)_CS_V7_ENV },
	};
	size_t i, j, n = sizeof c / sizeof c[0];

	pc_begin("XBD.unistd.h.016");
	pc_test();
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			int ok = 0;
			pc_check(ok || c[i].val != c[j].val, "%s and %s have the same value (%ld)", c[i].name, c[j].name, c[i].val);
		}
	pc_pass();
}
