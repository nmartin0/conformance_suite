/*
 * gai_strerror() of a value that is not one of the listed codes still returns a non-empty string.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gai_strerror.002
 */
#include "pc.h"
#include <netdb.h>
#include <string.h>

int main(void)
{
	const int codes[] = { EAI_AGAIN, EAI_BADFLAGS, EAI_FAIL, EAI_FAMILY, EAI_MEMORY, EAI_NONAME, EAI_OVERFLOW,
	    EAI_SERVICE, EAI_SOCKTYPE, EAI_SYSTEM, 0 };
	int v = 123456, again = 1;
	size_t i;
	const char *s;

	pc_begin("XSH.gai_strerror.002");
	while (again) {
		again = 0;
		for (i = 0; i < sizeof codes / sizeof codes[0]; i++)
			if (codes[i] == v) {
				v++;
				again = 1;
			}
	}
	pc_test();
	s = gai_strerror(v);
	pc_check(s != NULL, "gai_strerror(%d) returned a null pointer", v);
	pc_check(s[0] != '\0', "gai_strerror(%d) returned an empty string", v);
	pc_pass();
}
