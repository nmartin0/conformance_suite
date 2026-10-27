/*
 * The <regex.h> constants and types: distinct error values, separable flags, a wide signed regoff_t.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.regcomp.051
 */
#include "rx.h"

#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

int main(void)
{
	static const int errs[] = {
		REG_NOMATCH, REG_BADPAT, REG_ECOLLATE, REG_ECTYPE, REG_EESCAPE, REG_ESUBREG,
		REG_EBRACK, REG_EPAREN, REG_EBRACE, REG_BADBR, REG_ERANGE, REG_ESPACE, REG_BADRPT
	};
	static const int cfl[] = { REG_EXTENDED, REG_ICASE, REG_NOSUB, REG_NEWLINE };
	static const int efl[] = { REG_NOTBOL, REG_NOTEOL };
	regex_t re;
	regmatch_t m;
	size_t i, j;

	pc_begin("XSH.regcomp.051");
	pc_test();
	for (i = 0; i < RX_N(errs); i++) {
		pc_check(errs[i] != 0, "error constant %zu is zero", i);
		for (j = i + 1; j < RX_N(errs); j++)
			pc_check(errs[i] != errs[j], "error constants %zu and %zu are equal", i, j);
	}
	for (i = 0; i < RX_N(cfl); i++) {
		pc_check(cfl[i] != 0, "cflag %zu is zero", i);
		for (j = i + 1; j < RX_N(cfl); j++)
			pc_check((cfl[i] & cfl[j]) == 0, "cflags %zu and %zu overlap", i, j);
	}
	for (i = 0; i < RX_N(efl); i++)
		pc_check(efl[i] != 0, "eflag %zu is zero", i);
	pc_check((efl[0] & efl[1]) == 0, "REG_NOTBOL and REG_NOTEOL overlap");
	pc_check((regoff_t)-1 < 0, "regoff_t is not signed");
	pc_check((intmax_t)(regoff_t)PTRDIFF_MAX == (intmax_t)PTRDIFF_MAX, "regoff_t cannot hold PTRDIFF_MAX");
	pc_check((intmax_t)(regoff_t)SSIZE_MAX == (intmax_t)SSIZE_MAX, "regoff_t cannot hold SSIZE_MAX");
	pc_check(_Generic(re.re_nsub, size_t: 1, default: 0), "re_nsub is not a size_t");
	pc_check(_Generic(m.rm_so, regoff_t: 1, default: 0), "rm_so is not a regoff_t");
	pc_check(_Generic(m.rm_eo, regoff_t: 1, default: 0), "rm_eo is not a regoff_t");
	pc_pass();
}
