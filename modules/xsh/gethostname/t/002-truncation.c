/*
 * gethostname() with a buffer one short of the name writes no more than namelen bytes, and they are a prefix of the name.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gethostname.002
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	char full[4096], buf[4096 + 16];
	size_t len, n, i;
	int rc;

	pc_begin("XSH.gethostname.002");
	memset(full, 0, sizeof full);
	pc_check(gethostname(full, sizeof full) == 0, "gethostname() failed: %s", pc_errname(errno));
	len = strlen(full);
	if (len < 2)
		pc_notinuse("the host name has %lu character(s), too short to truncate", (unsigned long)len);
	n = len - 1;
	memset(buf, 0xA5, sizeof buf);
	pc_test();
	rc = gethostname(buf, n);
	for (i = n; i < n + 16; i++)
		pc_check((unsigned char)buf[i] == 0xA5, "byte %lu, past namelen, was written", (unsigned long)i);
	/* The returned name is truncated, so its first namelen bytes are there whatever the return value. */
	pc_check(memcmp(buf, full, n) == 0, "the %lu bytes written (return value %d) are not a prefix of the name", (unsigned long)n, rc);
	pc_pass();
}
