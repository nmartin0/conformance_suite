/*
 * wcsrtombs() stops when the next character would exceed len bytes, leaves
 * *src just past the last character converted and can be resumed; when len
 * equals the byte length no terminating null byte is stored and *src is not
 * null.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsrtombs.002
 */
#include "pc.h"
#include <string.h>
#include <wchar.h>

int main(void)
{
	static const wchar_t w1[] = { L'a', L'b', L'c', L'd', 0 }, w2[] = { L'a', L'b', L'c', 0 };
	mbstate_t st;
	const wchar_t *src;
	char d[16];
	size_t r1, r2, r3;
	const wchar_t *after1, *after2;

	pc_begin("XSH.wcsrtombs.002");
	pc_test();
	memset(&st, 0, sizeof st);
	src = w1;
	memset(d, 'x', sizeof d);
	r1 = wcsrtombs(d, &src, 2, &st);
	after1 = src;
	pc_check(r1 == 2, "wcsrtombs() with len 2 returned %zu, expected 2", r1);
	pc_check(d[0] == 'a' && d[1] == 'b', "wcsrtombs() stored the wrong bytes");
	pc_check(d[2] == 'x', "wcsrtombs() modified a byte beyond len");
	pc_check(after1 == w1 + 2, "wcsrtombs() left *src at the wrong place after len bytes");
	r2 = wcsrtombs(d + 8, &src, 8, &st);
	after2 = src;
	pc_check(r2 == 2 && d[8] == 'c' && d[9] == 'd', "resumed wcsrtombs() returned %zu or stored the wrong bytes", r2);
	pc_check(d[10] == '\0', "resumed wcsrtombs() did not store the terminating null byte");
	pc_check(after2 == NULL, "resumed wcsrtombs() left *src non-null after reaching the null wide character");
	memset(&st, 0, sizeof st);
	src = w2;
	memset(d, 'x', sizeof d);
	r3 = wcsrtombs(d, &src, 3, &st);
	pc_check(r3 == 3, "wcsrtombs() with len 3 on L\"abc\" returned %zu, expected 3", r3);
	pc_check(d[3] == 'x', "wcsrtombs() stored a terminating null byte after len bytes");
	pc_check(src == w2 + 3, "wcsrtombs() left *src at the wrong place when len ended the conversion");
	pc_pass();
}
