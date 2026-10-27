/*
 * gai_strerror() returns a non-empty string for each error value the page lists.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gai_strerror.001
 */
#include "pc.h"
#include <netdb.h>
#include <string.h>

int main(void)
{
	static const struct { int code; const char *name; } codes[] = {
		{ EAI_AGAIN, "EAI_AGAIN" }, { EAI_BADFLAGS, "EAI_BADFLAGS" }, { EAI_FAIL, "EAI_FAIL" },
		{ EAI_FAMILY, "EAI_FAMILY" }, { EAI_MEMORY, "EAI_MEMORY" }, { EAI_NONAME, "EAI_NONAME" },
		{ EAI_OVERFLOW, "EAI_OVERFLOW" }, { EAI_SERVICE, "EAI_SERVICE" }, { EAI_SOCKTYPE, "EAI_SOCKTYPE" },
		{ EAI_SYSTEM, "EAI_SYSTEM" },
	};
	size_t i;

	pc_begin("XSH.gai_strerror.001");
	pc_test();
	for (i = 0; i < sizeof codes / sizeof codes[0]; i++) {
		const char *s = gai_strerror(codes[i].code);

		pc_check(s != NULL, "gai_strerror(%s) returned a null pointer", codes[i].name);
		pc_check(s[0] != '\0', "gai_strerror(%s) returned an empty string", codes[i].name);
	}
	pc_pass();
}
