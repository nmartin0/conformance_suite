/*
 * The result of crypt() begins with the first two bytes of the salt.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.crypt.001
 */
#include "pc.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	static const char *const salt[] = { "ab", "Zz", "09", "./", "aZ09./xx", "zA" };
	static const char *const key[] = { "password", "", "a", "0123456789012345678901234567890123456789012345678901234567890123456789" };
	size_t i, j;
	char *r;

	pc_begin("XSH.crypt.001");
	pc_test();
	for (i = 0; i < sizeof salt / sizeof salt[0]; i++)
		for (j = 0; j < sizeof key / sizeof key[0]; j++) {
			errno = 0;
			r = crypt(key[j], salt[i]);
			if (r == NULL) {
				if (errno == ENOSYS)
					pc_notinuse("crypt() is not supported (ENOSYS)");
				pc_unresolved("crypt() returned a null pointer (errno %s) for a valid salt", pc_errname(errno));
			}
			pc_check(strlen(r) >= 2, "the result is shorter than two bytes");
			pc_check(r[0] == salt[i][0] && r[1] == salt[i][1],
			    "the result for salt %s begins with %c%c", salt[i], r[0], r[1]);
		}
	pc_pass();
}
