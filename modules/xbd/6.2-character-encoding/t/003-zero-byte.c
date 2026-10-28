/*
 * The zero byte is the null character whatever the shift state, and it is
 * never part of another character: it decodes alone, and after any byte
 * that starts a longer character it either ends the sequence as an error
 * or is the null character itself, never a continuation.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.6.2-character-encoding.003
 */
#include "pc.h"
#include "pcl.h"
#include <wchar.h>

int main(void)
{
	int k, lead;

	pc_begin("XBD.6.2-character-encoding.003");
	pcl_collect();
	pc_test();
	for (k = 0; k < pcl_count; k++) {
		mbstate_t st;
		wchar_t w = 9;
		char zero = '\0';
		size_t r;

		pc_check(setlocale(LC_ALL, pcl_name[k]) != NULL, "setlocale(\"%s\") failed after being accepted", pcl_name[k]);
		memset(&st, 0, sizeof st);
		r = mbrtowc(&w, &zero, 1, &st);
		pc_check(r == 0 && w == 0 && mbsinit(&st) != 0, "locale %s: the zero byte alone is not the null character",
		    pcl_name[k]);
		for (lead = 1; lead < 256; lead++) {
			char b = (char)lead;

			memset(&st, 0, sizeof st);
			r = mbrtowc(&w, &b, 1, &st);
			if (r != (size_t)-2)
				continue;
			w = 9;
			r = mbrtowc(&w, &zero, 1, &st);
			pc_check(r == 0 || r == (size_t)-1,
			    "locale %s: after the incomplete sequence starting with byte %d the zero byte gave %ld (a part of a character)",
			    pcl_name[k], lead, (long)r);
		}
	}
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
