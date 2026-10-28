/*
 * pcbat.h - a description of how the global locale behaves on the portable
 * characters (classes, case mappings, collation, langinfo, localeconv),
 * as one string, so two locales can be compared for sameness.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef PCBAT_H
#define PCBAT_H

#include "pcchars.h"
#include <ctype.h>
#include <langinfo.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>

#define PCB_SIZE 70000
static char pcb_a[PCB_SIZE], pcb_b[PCB_SIZE];

#define PCB_PUT(...) \
	do { \
		int w_ = snprintf(out + len, n - len, __VA_ARGS__); \
		if (w_ < 0 || (size_t)w_ >= n - len) { \
			out[n - 1] = '\0'; \
			return; \
		} \
		len += (size_t)w_; \
	} while (0)

/* pcc_init() must have run. with_codeset adds the code set name. */
static void pcb_sig(char *out, size_t n, int with_codeset)
{
	static const nl_item items[] = {
		ABDAY_1, ABDAY_2, ABDAY_3, ABDAY_4, ABDAY_5, ABDAY_6, ABDAY_7,
		DAY_1, DAY_2, DAY_3, DAY_4, DAY_5, DAY_6, DAY_7,
		ABMON_1, ABMON_2, ABMON_3, ABMON_4, ABMON_5, ABMON_6,
		ABMON_7, ABMON_8, ABMON_9, ABMON_10, ABMON_11, ABMON_12,
		MON_1, MON_2, MON_3, MON_4, MON_5, MON_6,
		MON_7, MON_8, MON_9, MON_10, MON_11, MON_12,
		AM_STR, PM_STR, D_T_FMT, D_FMT, T_FMT, T_FMT_AMPM,
		RADIXCHAR, YESEXPR, NOEXPR
	};
	size_t len = 0, k;
	const struct lconv *lc;
	int i, j;

	out[0] = '\0';
	if (with_codeset)
		PCB_PUT("[%s]", nl_langinfo(CODESET));
	for (i = 0; i < PCC_N; i++) {
		int c = (unsigned char)pcc_all[i];
		wint_t w = btowc(c);
		unsigned f = 0, g = 0;

		f |= isalnum(c) ? 0x001u : 0u;
		f |= isalpha(c) ? 0x002u : 0u;
		f |= isblank(c) ? 0x004u : 0u;
		f |= iscntrl(c) ? 0x008u : 0u;
		f |= isdigit(c) ? 0x010u : 0u;
		f |= isgraph(c) ? 0x020u : 0u;
		f |= islower(c) ? 0x040u : 0u;
		f |= isprint(c) ? 0x080u : 0u;
		f |= ispunct(c) ? 0x100u : 0u;
		f |= isspace(c) ? 0x200u : 0u;
		f |= isupper(c) ? 0x400u : 0u;
		f |= isxdigit(c) ? 0x800u : 0u;
		g |= iswalnum(w) ? 0x001u : 0u;
		g |= iswalpha(w) ? 0x002u : 0u;
		g |= iswblank(w) ? 0x004u : 0u;
		g |= iswcntrl(w) ? 0x008u : 0u;
		g |= iswdigit(w) ? 0x010u : 0u;
		g |= iswgraph(w) ? 0x020u : 0u;
		g |= iswlower(w) ? 0x040u : 0u;
		g |= iswprint(w) ? 0x080u : 0u;
		g |= iswpunct(w) ? 0x100u : 0u;
		g |= iswspace(w) ? 0x200u : 0u;
		g |= iswupper(w) ? 0x400u : 0u;
		g |= iswxdigit(w) ? 0x800u : 0u;
		PCB_PUT("%03x%02x%02x%03x%x%x;", f, (unsigned)toupper(c), (unsigned)tolower(c), g, (unsigned)towupper(w),
		    (unsigned)towlower(w));
	}
	PCB_PUT("|");
	for (i = 0; i < PCC_N; i++) {
		for (j = 0; j < PCC_N; j++) {
			char s1[2], s2[2];
			int s;

			s1[0] = pcc_all[i];
			s1[1] = '\0';
			s2[0] = pcc_all[j];
			s2[1] = '\0';
			s = strcoll(s1, s2);
			PCB_PUT("%c", s < 0 ? '<' : s > 0 ? '>' : '=');
		}
	}
	PCB_PUT("|");
	for (k = 0; k < sizeof items / sizeof items[0]; k++)
		PCB_PUT("%s|", nl_langinfo(items[k]));
	lc = localeconv();
	PCB_PUT("%s|%s|%s|", lc->decimal_point, lc->thousands_sep, lc->grouping);
	PCB_PUT("%s|%s|%s|%s|%s|%s|", lc->int_curr_symbol, lc->currency_symbol, lc->mon_decimal_point,
	    lc->mon_thousands_sep, lc->mon_grouping, lc->positive_sign);
	PCB_PUT("%s|", lc->negative_sign);
	PCB_PUT("%d,%d,%d,%d,%d,%d,%d,%d,", lc->int_frac_digits, lc->frac_digits, lc->p_cs_precedes, lc->p_sep_by_space,
	    lc->n_cs_precedes, lc->n_sep_by_space, lc->p_sign_posn, lc->n_sign_posn);
	PCB_PUT("%d,%d,%d,%d,%d,%d", lc->int_p_cs_precedes, lc->int_p_sep_by_space, lc->int_n_cs_precedes,
	    lc->int_n_sep_by_space, lc->int_p_sign_posn, lc->int_n_sign_posn);
}

/* Index of the first byte at which a and b differ. */
static size_t pcb_diff(const char *a, const char *b)
{
	size_t i = 0;

	while (a[i] != '\0' && a[i] == b[i])
		i++;
	return i;
}

/* Which part of the description the offset falls in. */
static const char *pcb_part(const char *a, size_t off)
{
	size_t i, bars = 0;

	for (i = 0; i < off && a[i] != '\0'; i++)
		if (a[i] == '|')
			bars++;
	if (bars == 0)
		return "the character classes or case mappings of the portable characters";
	if (bars == 1)
		return "the collation order of the portable characters";
	return "the langinfo or localeconv values";
}

#endif
