/*
 * While in the initial shift state the portable characters keep their
 * usual interpretation and do not alter the shift state: in every locale
 * the state is initial after decoding or encoding any of them.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.6.2-character-encoding.002
 */
#include "pc.h"
#include "pcchars.h"
#include "pcl.h"
#include <wchar.h>

int main(void)
{
	int i, k;

	pc_begin("XBD.6.2-character-encoding.002");
	pcc_init();
	pcl_collect();
	pc_test();
	for (k = 0; k < pcl_count; k++) {
		pc_check(setlocale(LC_ALL, pcl_name[k]) != NULL, "setlocale(\"%s\") failed after being accepted", pcl_name[k]);
		for (i = 0; i < PCC_N; i++) {
			mbstate_t st;
			char buf[MB_LEN_MAX];
			wchar_t w = 5;
			size_t r;

			memset(&st, 0, sizeof st);
			r = wcrtomb(buf, (wchar_t)pcc_all[i], &st);
			pc_check(r == 1, "locale %s: member %d does not encode as one byte", pcl_name[k], i);
			pc_check(mbsinit(&st) != 0, "locale %s: encoding member %d left the state non-initial", pcl_name[k], i);
			memset(&st, 0, sizeof st);
			r = mbrtowc(&w, buf, 1, &st);
			pc_check(r == (pcc_all[i] == '\0' ? 0u : 1u), "locale %s: member %d does not decode alone", pcl_name[k], i);
			pc_check(mbsinit(&st) != 0, "locale %s: decoding member %d left the state non-initial", pcl_name[k], i);
		}
	}
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
