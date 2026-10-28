/*
 * The most bytes in a character in the current locale is MB_CUR_MAX and
 * the implementation's most is MB_LEN_MAX: in every locale MB_CUR_MAX is at
 * least 1 and no more than MB_LEN_MAX, and a character is never longer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.6.2-character-encoding.005
 */
#include "pc.h"
#include "pcl.h"
#include <wchar.h>

int main(void)
{
	int k, b;

	pc_begin("XBD.6.2-character-encoding.005");
	pcl_collect();
	pc_test();
	for (k = 0; k < pcl_count; k++) {
		pc_check(setlocale(LC_ALL, pcl_name[k]) != NULL, "setlocale(\"%s\") failed after being accepted", pcl_name[k]);
		pc_check(MB_CUR_MAX >= 1 && MB_CUR_MAX <= (size_t)MB_LEN_MAX, "locale %s: MB_CUR_MAX is %lu, MB_LEN_MAX is %d",
		    pcl_name[k], (unsigned long)MB_CUR_MAX, MB_LEN_MAX);
		for (b = 0; b < 256; b++) {
			mbstate_t st;
			char buf[MB_LEN_MAX + 1];
			wchar_t w;
			size_t r;

			memset(&st, 0, sizeof st);
			memset(buf, b, sizeof buf);
			r = mbrtowc(&w, buf, sizeof buf, &st);
			if (r != (size_t)-1 && r != (size_t)-2)
				pc_check(r <= MB_CUR_MAX, "locale %s: byte %d starts a character of %lu bytes, MB_CUR_MAX is %lu",
				    pcl_name[k], b, (unsigned long)r, (unsigned long)MB_CUR_MAX);
		}
	}
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
