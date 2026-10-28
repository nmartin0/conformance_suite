/*
 * When the listed characters of the POSIX collation sequence are encoded as
 * ASCII, all 256 bytes collate in the order of their values: the empty
 * string first, then byte 1 up to byte 255, through strcoll() and
 * wcscoll().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.2-lc-collate.002
 */
#include "pc.h"
#include "pcchars.h"
#include <ctype.h>
#include <locale.h>
#include <string.h>
#include <wchar.h>

int main(void)
{
	size_t i;
	int b;

	pc_begin("XBD.7.3.2-lc-collate.002");
	pcc_init();
	pc_check(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
	/* ASCII when the printable members sit at 32 + position and the controls at 7 to 13 and 0 */
	for (i = 0; pcc_order[i + 7] != '\0'; i++)
		if ((unsigned char)pcc_order[i + 7] != 32 + i)
			pc_notinuse("the portable characters are not ASCII-encoded");
	if ((unsigned char)'\a' != 7 || (unsigned char)'\b' != 8 || (unsigned char)'\t' != 9 || (unsigned char)'\n' != 10 ||
	    (unsigned char)'\v' != 11 || (unsigned char)'\f' != 12 || (unsigned char)'\r' != 13)
		pc_notinuse("the portable controls are not ASCII-encoded");
	for (b = 1; b < 32; b++)
		if (!iscntrl(b))
			pc_notinuse("byte %d is not a control character, so the other controls are not ASCII-encoded", b);
	if (!iscntrl(127))
		pc_notinuse("byte 127 is not a control character, so the set is not ASCII-encoded");
	pc_test();
	pc_check(strcoll("", "\x01") < 0, "the empty string does not collate before byte 1");
	for (b = 1; b < 255; b++) {
		char s1[2], s2[2];
		wchar_t w1[2], w2[2];

		s1[0] = (char)b;
		s1[1] = '\0';
		s2[0] = (char)(b + 1);
		s2[1] = '\0';
		pc_check(strcoll(s1, s2) < 0, "byte %d does not collate before byte %d", b, b + 1);
		w1[0] = (wchar_t)btowc(b);
		w1[1] = 0;
		w2[0] = (wchar_t)btowc(b + 1);
		w2[1] = 0;
		pc_check(wcscoll(w1, w2) < 0, "wcscoll: the code of byte %d does not collate before that of byte %d", b, b + 1);
	}
	pc_pass();
}
