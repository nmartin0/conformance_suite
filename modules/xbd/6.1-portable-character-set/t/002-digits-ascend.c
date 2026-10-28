/*
 * The encoded values of the digits 0 to 9 ascend by one, in the POSIX
 * locale and in every other locale the system offers.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.6.1-portable-character-set.002
 */
#include "pc.h"
#include "pcchars.h"
#include "pcl.h"
#include <locale.h>
#include <string.h>
#include <wchar.h>

int main(void)
{
	int i, k;

	pc_begin("XBD.6.1-portable-character-set.002");
	pcc_init();
	pcl_collect();
	pc_test();
	for (k = 0; k < pcl_count; k++) {
		unsigned char prev = 0;

		pc_check(setlocale(LC_ALL, pcl_name[k]) != NULL, "setlocale(\"%s\") failed after being accepted", pcl_name[k]);
		for (i = 0; i < 10; i++) {
			mbstate_t st;
			char buf[MB_LEN_MAX];
			size_t r;

			memset(&st, 0, sizeof st);
			r = wcrtomb(buf, (wchar_t)pcc_digit[i], &st);
			pc_check(r == 1, "locale %s: digit %d is not encoded as one byte (wcrtomb gave %lu)", pcl_name[k], i,
			    (unsigned long)r);
			if (i > 0)
				pc_check((unsigned char)buf[0] == (unsigned char)(prev + 1),
				    "locale %s: digit %d is byte %u, digit %d is byte %u: not one apart", pcl_name[k], i - 1,
				    (unsigned)prev, i, (unsigned)(unsigned char)buf[0]);
			prev = (unsigned char)buf[0];
		}
	}
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
