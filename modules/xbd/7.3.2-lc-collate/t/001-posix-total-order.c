/*
 * The POSIX locale collates every pair of distinct bytes in some order
 * (strcoll never gives 0, the order is a consistent linear one, strxfrm()
 * and wcscoll() agree with it), and every byte that is neither a portable
 * member nor a control character collates after all portable members.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.3.2-lc-collate.001
 */
#include "pc.h"
#include "pcchars.h"
#include <ctype.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

static int rank_of[256];

static int sgn(int v)
{
	return v < 0 ? -1 : v > 0;
}

static int cmp_bytes(const void *a, const void *b)
{
	char s1[2], s2[2];

	s1[0] = (char)*(const int *)a;
	s1[1] = '\0';
	s2[0] = (char)*(const int *)b;
	s2[1] = '\0';
	return strcoll(s1, s2);
}

int main(void)
{
	int sorted[255];
	int b, c, i;

	pc_begin("XBD.7.3.2-lc-collate.001");
	pcc_init();
	pc_check(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
	for (b = 1; b < 256; b++)
		sorted[b - 1] = b;
	pc_test();
	qsort(sorted, 255, sizeof sorted[0], cmp_bytes);
	for (i = 0; i < 255; i++)
		rank_of[sorted[i]] = i;
	for (b = 1; b < 256; b++) {
		for (c = 1; c < 256; c++) {
			char s1[2], s2[2], x1[300], x2[300];
			wchar_t w1[2], w2[2];
			int r = sgn(strcoll((s1[0] = (char)b, s1[1] = '\0', s1), (s2[0] = (char)c, s2[1] = '\0', s2)));

			if (b == c) {
				pc_check(r == 0, "strcoll of byte %d with itself is not 0", b);
				continue;
			}
			pc_check(r != 0, "bytes %d and %d collate as equal", b, c);
			pc_check(r == sgn(rank_of[b] - rank_of[c]), "the order of bytes %d and %d is not consistent with a linear order", b, c);
			pc_check(r == -sgn(strcoll(s2, s1)), "strcoll is not antisymmetric for bytes %d and %d", b, c);
			if (strxfrm(x1, s1, sizeof x1) < sizeof x1 && strxfrm(x2, s2, sizeof x2) < sizeof x2)
				pc_check(sgn(strcmp(x1, x2)) == r, "strxfrm disagrees with strcoll for bytes %d and %d", b, c);
			w1[0] = (wchar_t)btowc(b);
			w1[1] = 0;
			w2[0] = (wchar_t)btowc(c);
			w2[1] = 0;
			pc_check(sgn(wcscoll(w1, w2)) == r, "wcscoll disagrees with strcoll for bytes %d and %d", b, c);
		}
	}
	for (b = 1; b < 256; b++) {
		char u[2];

		if ((pcc_flags[b] & PCC_PORT) != 0 || iscntrl(b))
			continue;
		u[0] = (char)b;
		u[1] = '\0';
		for (i = 1; i < PCC_N; i++) {
			char p[2];

			p[0] = pcc_all[i];
			p[1] = '\0';
			pc_check(strcoll(p, u) < 0, "byte %d, which is neither a portable member nor a control, collates before a portable member", b);
		}
	}
	pc_pass();
}
