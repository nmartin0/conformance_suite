/*
 * A va_list can be passed to vsnprintf().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.va_start.005
 */
#include "pc.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static char out[128];
static int rc;

static void fmt(const char *f, ...)
{
	va_list ap;

	va_start(ap, f);
	rc = vsnprintf(out, sizeof out, f, ap);
	va_end(ap);
}

int main(void)
{
	pc_begin("XSH.va_start.005");
	pc_test();
	fmt("%d-%s-%.2f-%c", 42, "abc", 1.5, 'x');
	pc_check(strcmp(out, "42-abc-1.50-x") == 0, "vsnprintf() through a va_list gave \"%s\"", out);
	pc_check(rc == 13, "vsnprintf() returned %d, expected 13", rc);
	pc_pass();
}
