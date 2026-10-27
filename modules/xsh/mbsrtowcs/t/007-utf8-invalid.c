/*
 * mbsrtowcs() stops at an invalid byte with (size_t)-1 and EILSEQ, having
 * stored the characters before it and left *src just past them.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbsrtowcs.007
 */
#include "pc.h"
#include <errno.h>
#include <locale.h>
#include <string.h>
#include <wchar.h>

static void use_utf8(void)
{
	const char *n = pc_find_locale("UTF-8", 1);

	if (n == NULL)
		pc_notinuse("no UTF-8 locale installed");
	if (setlocale(LC_ALL, n) == NULL)
		pc_unresolved("setlocale(LC_ALL, \"%s\") failed", n);
}

int main(void)
{
	static const char s[] = "a\x80" "b";
	mbstate_t st;
	const char *src = s;
	wchar_t w[8];
	size_t r, i;
	int e;

	pc_begin("XSH.mbsrtowcs.007");
	use_utf8();
	memset(&st, 0, sizeof st);
	for (i = 0; i < 8; i++)
		w[i] = 77;
	pc_test();
	errno = EDOM;
	r = mbsrtowcs(w, &src, 8, &st);
	e = errno;
	pc_check(r == (size_t)-1, "mbsrtowcs() returned %zu, expected (size_t)-1", r);
	pc_check(e == EILSEQ, "mbsrtowcs() set errno to %s, expected EILSEQ", pc_errname(e));
	pc_check(w[0] == L'a', "mbsrtowcs() did not store the character before the invalid byte");
	pc_check(src == s + 1, "mbsrtowcs() left *src at %s, expected just past the last converted character", src == NULL ? "a null pointer" : "another offset");
	pc_pass();
}
