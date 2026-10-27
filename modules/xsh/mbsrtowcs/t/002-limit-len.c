/*
 * mbsrtowcs() stops after len codes, leaves *src just past the last
 * character converted and can be resumed; when len equals the string
 * length no terminating zero is stored and *src is not null.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mbsrtowcs.002
 */
#include "pc.h"
#include <string.h>
#include <wchar.h>

int main(void)
{
	static const char s1[] = "abcd", s2[] = "abc";
	mbstate_t st;
	const char *src;
	wchar_t w[8];
	size_t r1, r2, r3, i;
	const char *after1, *after2;

	pc_begin("XSH.mbsrtowcs.002");
	pc_test();
	memset(&st, 0, sizeof st);
	src = s1;
	for (i = 0; i < 8; i++)
		w[i] = 77;
	r1 = mbsrtowcs(w, &src, 2, &st);
	after1 = src;
	pc_check(r1 == 2, "mbsrtowcs() with len 2 returned %zu, expected 2", r1);
	pc_check(w[0] == L'a' && w[1] == L'b', "mbsrtowcs() stored the wrong codes");
	pc_check(w[2] == 77, "mbsrtowcs() modified an element beyond len");
	pc_check(after1 == s1 + 2, "mbsrtowcs() left *src at offset %ld, expected 2", (long)(after1 - s1));
	r2 = mbsrtowcs(w + 4, &src, 4, &st);
	after2 = src;
	pc_check(r2 == 2 && w[4] == L'c' && w[5] == L'd', "resumed mbsrtowcs() returned %zu or stored the wrong codes", r2);
	pc_check(w[6] == 0, "resumed mbsrtowcs() did not store the terminating zero");
	pc_check(after2 == NULL, "resumed mbsrtowcs() left *src non-null after reaching the null byte");
	memset(&st, 0, sizeof st);
	src = s2;
	for (i = 0; i < 8; i++)
		w[i] = 77;
	r3 = mbsrtowcs(w, &src, 3, &st);
	pc_check(r3 == 3, "mbsrtowcs() with len 3 on \"abc\" returned %zu, expected 3", r3);
	pc_check(w[3] == 77, "mbsrtowcs() stored a terminating zero after len codes");
	pc_check(src == s2 + 3, "mbsrtowcs() left *src at the wrong place when len ended the conversion");
	pc_pass();
}
