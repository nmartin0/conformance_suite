/*
 * The POSIX locale behaves as if it had been defined with localedef from
 * the POSIX locale tables: a locale built from those tables (for the
 * portable characters) answers exactly like the POSIX locale for classes,
 * case mappings, collation, langinfo items and localeconv().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.2-posix-locale.002
 */
#include "pc.h"
#include "ldef.h"
#include "pcbat.h"

#define SIZE 12000

static char *add(char *dst, size_t n, const char *fmt, const char *a)
{
	size_t len = strlen(dst);

	(void)snprintf(dst + len, n - len, fmt, a);
	return dst;
}

int main(void)
{
	static char src[SIZE];
	char l1[1400], l2[1400], l3[1400], l4[1400], l5[1400], l6[1400], l7[1400], loc[4200], nb[4];
	char s1[300], s2[300], s3[300], s4[300];
	char order[1800];
	size_t d;
	const char *p;
	int i;

	pc_begin("XBD.7.2-posix-locale.002");
	ld_need();
	pcc_init();
	ld_charmap("cmap", LD_HDR, '\\', "", "", 0);
	src[0] = '\0';
	add(src, SIZE, "LC_CTYPE\nupper %s\n", pcc_list(l1, sizeof l1, pcc_upper));
	add(src, SIZE, "lower %s\n", pcc_list(l2, sizeof l2, pcc_lower));
	add(src, SIZE, "digit %s\n", pcc_list(l3, sizeof l3, pcc_digit));
	add(src, SIZE, "space %s\n", pcc_list(l4, sizeof l4, pcc_space6));
	add(src, SIZE, "cntrl %s;<NUL>\n", pcc_list(l5, sizeof l5, pcc_ctl7));
	add(src, SIZE, "punct %s\n", pcc_list(l6, sizeof l6, pcc_punct));
	add(src, SIZE, "xdigit %s;", pcc_list(l7, sizeof l7, pcc_digit));
	add(src, SIZE, "%s;", pcc_list(l1, sizeof l1, pcc_xup));
	add(src, SIZE, "%s\n", pcc_list(l2, sizeof l2, pcc_xlow));
	add(src, SIZE, "blank %s\n", pcc_list(l3, sizeof l3, " \t"));
	add(src, SIZE, "toupper %s\n", pcc_pairs(l4, sizeof l4, pcc_lower, pcc_upper));
	add(src, SIZE, "tolower %s\nEND LC_CTYPE\n", pcc_pairs(l5, sizeof l5, pcc_upper, pcc_lower));
	/* collation: the controls, then the printable members, in the listed order */
	order[0] = '\0';
	(void)strcat(order, "<NUL>\n");
	for (p = pcc_order; *p != '\0'; p++) {
		(void)strcat(order, "<");
		(void)strcat(order, pcc_sym(*p, nb));
		(void)strcat(order, ">\n");
	}
	add(src, SIZE, "LC_COLLATE\norder_start forward\n%s", order);
	add(src, SIZE, "%s", "order_end\nEND LC_COLLATE\n");
	add(src, SIZE, "%s",
	    "LC_MONETARY\nint_curr_symbol \"\"\ncurrency_symbol \"\"\nmon_decimal_point \"\"\nmon_thousands_sep \"\"\n"
	    "positive_sign \"\"\nnegative_sign \"\"\nint_frac_digits -1\nfrac_digits -1\np_cs_precedes -1\np_sep_by_space -1\n"
	    "n_cs_precedes -1\nn_sep_by_space -1\np_sign_posn -1\nn_sign_posn -1\nint_p_cs_precedes -1\n"
	    "int_p_sep_by_space -1\nint_n_cs_precedes -1\nint_n_sep_by_space -1\nint_p_sign_posn -1\n"
	    "int_n_sign_posn -1\nEND LC_MONETARY\n");
	add(src, SIZE, "%s", "LC_NUMERIC\ndecimal_point \"<period>\"\nthousands_sep \"\"\nEND LC_NUMERIC\n");
	add(src, SIZE, "LC_TIME\nabday %s;", pcc_str(s1, sizeof s1, "Sun"));
	{
		static const char *const ab[] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

		for (i = 0; i < 6; i++)
			add(src, SIZE, i < 5 ? "%s;" : "%s\n", pcc_str(s1, sizeof s1, ab[i]));
	}
	{
		static const char *const dn[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
		static const char *const abm[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
		static const char *const mn[] = { "January", "February", "March", "April", "May", "June", "July", "August",
		    "September", "October", "November", "December" };

		add(src, SIZE, "%s", "day ");
		for (i = 0; i < 7; i++)
			add(src, SIZE, i < 6 ? "%s;" : "%s\n", pcc_str(s1, sizeof s1, dn[i]));
		add(src, SIZE, "%s", "abmon ");
		for (i = 0; i < 12; i++)
			add(src, SIZE, i < 11 ? "%s;" : "%s\n", pcc_str(s1, sizeof s1, abm[i]));
		add(src, SIZE, "%s", "mon ");
		for (i = 0; i < 12; i++)
			add(src, SIZE, i < 11 ? "%s;" : "%s\n", pcc_str(s1, sizeof s1, mn[i]));
	}
	add(src, SIZE, "d_t_fmt %s\n", pcc_str(s1, sizeof s1, "%a %b %e %H:%M:%S %Y"));
	add(src, SIZE, "d_fmt %s\n", pcc_str(s2, sizeof s2, "%m/%d/%y"));
	add(src, SIZE, "t_fmt %s\n", pcc_str(s3, sizeof s3, "%H:%M:%S"));
	add(src, SIZE, "am_pm %s;", pcc_str(s4, sizeof s4, "AM"));
	add(src, SIZE, "%s\n", pcc_str(s4, sizeof s4, "PM"));
	add(src, SIZE, "t_fmt_ampm %s\nEND LC_TIME\n", pcc_str(s1, sizeof s1, "%I:%M:%S %p"));
	add(src, SIZE, "LC_MESSAGES\nyesexpr %s\n", pcc_str(s1, sizeof s1, "^[yY]"));
	add(src, SIZE, "noexpr %s\nEND LC_MESSAGES\n", pcc_str(s2, sizeof s2, "^[nN]"));
	ld_ok("tables", "cmap", src, loc, sizeof loc);
	pc_check(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
	pcb_sig(pcb_a, sizeof pcb_a, 0);
	pc_test();
	ld_load(loc);
	pcb_sig(pcb_b, sizeof pcb_b, 0);
	d = pcb_diff(pcb_a, pcb_b);
	pc_check(strcmp(pcb_a, pcb_b) == 0, "the locale built from the POSIX tables differs from the POSIX locale in %s (offset %lu)",
	    pcb_part(pcb_a, d), (unsigned long)d);
	pc_pass();
}
