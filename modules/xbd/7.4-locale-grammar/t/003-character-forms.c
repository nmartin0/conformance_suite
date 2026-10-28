/*
 * A character may be written in a locale source as an octal, hexadecimal
 * or decimal encoding of its bytes, as well as by its symbolic name: the
 * three forms, given the encoding of the comma, the period and the semicolon, read as
 * those characters in strings of the numeric and monetary categories and
 * in a class list.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.7.4-locale-grammar.003
 */
#include "pc.h"
#include "ldef.h"
#include <ctype.h>
#include <monetary.h>

#define REST LD_CP_COLLATE LD_CP_TIME LD_CP_MESSAGES

int main(void)
{
	static char text[8000];
	char loc[4200], cls[400];
	const struct lconv *lc;
	unsigned comma = (unsigned char)',', period = (unsigned char)'.', semi = (unsigned char)';';
	int n, i;

	pc_begin("XBD.7.4-locale-grammar.003");
	ld_need();
	pcc_init();
	for (i = 0; i < PCC_N; i++)
		if ((unsigned char)pcc_all[i] >= 0xe1 && (unsigned char)pcc_all[i] <= 0xe4)
			pc_notinuse("a portable character is encoded in the high range the test uses");
	ld_charmap("cmap", LD_HDR, '\\', "<x-h> \\xe1\n<x-d> \\xe2\n<x-o> \\xe3\n<x-n> \\xe4\n", "", 0);
	n = snprintf(cls, sizeof cls, "\\x%02X;\\d%u;\\%03o", 0xe1u, 0xe2u, 0xe3u);
	(void)n;
	(void)snprintf(text, sizeof text,
	    "LC_CTYPE\n"
	    "upper <A>;<B>;<C>;<D>;<E>;<F>;<G>;<H>;<I>;<J>;<K>;<L>;<M>;<N>;<O>;<P>;<Q>;<R>;<S>;<T>;<U>;<V>;<W>;<X>;<Y>;<Z>\n"
	    "lower %s;<a>;<b>;<c>;<d>;<e>;<f>;<g>;<h>;<i>;<j>;<k>;<l>;<m>;<n>;<o>;<p>;<q>;<r>;<s>;<t>;<u>;<v>;<w>;<x>;<y>;<z>\n"
	    "alpha <A>;<B>;<C>;<D>;<E>;<F>;<G>;<H>;<I>;<J>;<K>;<L>;<M>;<N>;<O>;<P>;<Q>;<R>;<S>;<T>;<U>;<V>;<W>;<X>;<Y>;<Z>;"
	    "<a>;<b>;<c>;<d>;<e>;<f>;<g>;<h>;<i>;<j>;<k>;<l>;<m>;<n>;<o>;<p>;<q>;<r>;<s>;<t>;<u>;<v>;<w>;<x>;<y>;<z>\n"
	    "digit <zero>;<one>;<two>;<three>;<four>;<five>;<six>;<seven>;<eight>;<nine>\n"
	    "space <space>;<form-feed>;<newline>;<carriage-return>;<tab>;<vertical-tab>\n"
	    "cntrl <alert>;<backspace>;<tab>;<newline>;<vertical-tab>;<form-feed>;<carriage-return>\n"
	    "punct <exclamation-mark>;<comma>;<period>\n"
	    "xdigit <zero>;<one>;<two>;<three>;<four>;<five>;<six>;<seven>;<eight>;<nine>;<A>;<B>;<C>;<D>;<E>;<F>;<a>;<b>;<c>;<d>;<e>;<f>\n"
	    "blank <space>;<tab>\n"
	    "toupper (<a>,<A>);(<b>,<B>);(<c>,<C>)\n"
	    "tolower (<A>,<a>);(<B>,<b>);(<C>,<c>)\n"
	    "END LC_CTYPE\n"
	    "LC_NUMERIC\ndecimal_point \"\\x%02X\"\nthousands_sep \"\\d%u\"\ngrouping 3\nEND LC_NUMERIC\n"
	    "LC_MONETARY\nmon_decimal_point \"\\%03o\"\nmon_thousands_sep \"\\x%02X\"\nmon_grouping 3\n"
	    "currency_symbol \"<Q>\"\nfrac_digits 2\np_cs_precedes 1\np_sep_by_space 0\nn_cs_precedes 1\nn_sep_by_space 0\n"
	    "p_sign_posn 1\nn_sign_posn 1\npositive_sign \"\"\nnegative_sign \"<hyphen-minus>\"\nEND LC_MONETARY\n%s",
	    cls, comma, period, comma, semi, REST);
	ld_ok("forms", "cmap", text, loc, sizeof loc);
	pc_test();
	ld_load(loc);
	lc = localeconv();
	pc_check(strcmp(lc->decimal_point, ",") == 0, "decimal_point written in hexadecimal is \"%s\", not \",\"", lc->decimal_point);
	pc_check(strcmp(lc->thousands_sep, ".") == 0, "thousands_sep written in decimal is \"%s\", not \".\"", lc->thousands_sep);
	pc_check(strcmp(lc->mon_decimal_point, ",") == 0, "mon_decimal_point written in octal is \"%s\", not \",\"", lc->mon_decimal_point);
	pc_check(strcmp(lc->mon_thousands_sep, ";") == 0, "mon_thousands_sep written in hexadecimal is \"%s\", not \";\"", lc->mon_thousands_sep);
	pc_check(islower(0xe1), "byte 0xe1, written as a hexadecimal encoding in the lower list, is not lower-case");
	pc_check(islower(0xe2), "byte 0xe2, written as a decimal encoding in the lower list, is not lower-case");
	pc_check(islower(0xe3), "byte 0xe3, written as an octal encoding in the lower list, is not lower-case");
	pc_check(!islower(0xe4) && !islower(0xe0), "a byte the lower list does not name is lower-case");
	pc_check(toupper((unsigned char)'a') == 'A' && toupper((unsigned char)'c') == 'C', "toupper of a or c is wrong");
	ld_unset_all();
	(void)setlocale(LC_ALL, "C");
	pc_pass();
}
