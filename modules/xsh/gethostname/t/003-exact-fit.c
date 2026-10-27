/*
 * gethostname() with a buffer of exactly the name length plus one gives the whole name, null-terminated.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gethostname.003
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	char full[4096], buf[4096];
	size_t len;

	pc_begin("XSH.gethostname.003");
	memset(full, 0, sizeof full);
	pc_check(gethostname(full, sizeof full) == 0, "gethostname() failed: %s", pc_errname(errno));
	len = strlen(full);
	memset(buf, 0xA5, sizeof buf);
	pc_test();
	pc_check(gethostname(buf, len + 1) == 0, "gethostname() with namelen %lu failed: %s", (unsigned long)(len + 1), pc_errname(errno));
	pc_check(strcmp(buf, full) == 0, "the name is \"%s\", expected \"%s\"", buf, full);
	pc_check((unsigned char)buf[len + 1] == 0xA5, "a byte past namelen was written");
	pc_pass();
}
