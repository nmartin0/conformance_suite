/*
 * putenv.c - conforming variants of putenv(), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "pcvariant.h"

/*
 * "name_einval": a string that is a valid name with no '=' makes putenv()
 * fail with EINVAL, leaving the environment alone. XSH putenv()
 * DESCRIPTION (since Issue 8) allows either removing the variable or
 * failing with [EINVAL]; the systems we try mostly remove it.
 */
static int valid_name(const char *s)
{
	if (*s == '\0' || (*s >= '0' && *s <= '9'))
		return 0;
	for (; *s != '\0'; s++)
		if (!(*s == '_' || (*s >= '0' && *s <= '9') || (*s >= 'A' && *s <= 'Z') || (*s >= 'a' && *s <= 'z')))
			return 0;
	return 1;
}

int putenv(char *string)
{
	const char *v = variant_of("putenv.");
	REAL(putenv);

	if (IS("name_einval") && valid_name(string)) {
		errno = EINVAL;
		return -1;
	}
	return real_putenv(string);
}
