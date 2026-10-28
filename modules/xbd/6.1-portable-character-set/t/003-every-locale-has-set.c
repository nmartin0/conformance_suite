/*
 * Every locale the system offers includes the portable character set:
 * each member is encoded as one byte that decodes back to the same
 * character; the members are distinct and the null character is the zero
 * byte.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.6.1-portable-character-set.003
 */
#include "pc.h"
#include "pcchars.h"
#include "pcl.h"
#include <limits.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

int main(void)
{
	int i, k;

	pc_begin("XBD.6.1-portable-character-set.003");
	pcc_init();
	pcl_collect();
	pc_test();
	for (k = 0; k < pcl_count; k++) {
		unsigned char seen[256];

		pc_check(setlocale(LC_ALL, pcl_name[k]) != NULL, "setlocale(\"%s\") failed after being accepted", pcl_name[k]);
		memset(seen, 0, sizeof seen);
		for (i = 0; i < PCC_N; i++) {
			mbstate_t st, st2;
			char buf[MB_LEN_MAX];
			wchar_t w = 7;
			size_t r;

			memset(&st, 0, sizeof st);
			r = wcrtomb(buf, (wchar_t)pcc_all[i], &st);
			pc_check(r == 1, "locale %s: member %d is not encoded as one byte (wcrtomb gave %lu)", pcl_name[k], i,
			    (unsigned long)r);
			if (pcc_all[i] == '\0')
				pc_check(buf[0] == '\0', "locale %s: the null character is not the zero byte", pcl_name[k]);
			pc_check(seen[(unsigned char)buf[0]] == 0, "locale %s: two members share byte %u", pcl_name[k],
			    (unsigned)(unsigned char)buf[0]);
			seen[(unsigned char)buf[0]] = 1;
			memset(&st2, 0, sizeof st2);
			r = mbrtowc(&w, buf, 1, &st2);
			pc_check(r == (pcc_all[i] == '\0' ? 0u : 1u), "locale %s: member %d: mbrtowc of its byte gave %lu", pcl_name[k],
			    i, (unsigned long)r);
			pc_check(w == (wchar_t)pcc_all[i], "locale %s: member %d decodes to another code", pcl_name[k], i);
		}
	}
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
