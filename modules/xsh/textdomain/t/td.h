/*
 * td.h - shared includes for the text domain tests.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef TD_H
#define TD_H

#include "pc.h"
#include <errno.h>
#include <iconv.h>
#include <langinfo.h>
#include <libintl.h>
#include <limits.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static inline const char *td_cwd(void)
{
	static char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof cwd) == NULL)
		pc_unresolved("getcwd failed");
	return cwd;
}

/*
 * Code set names that bind_textdomain_codeset() may be given: the first
 * one that iconv_open() accepts as a target. Returns how many of the
 * candidates are usable, copying up to two of them into out.
 */
static inline int td_codesets(const char **out)
{
	static const char *const cand[] = { "UTF-8", "US-ASCII", "ISO-8859-1" };
	int n = 0;
	size_t i, j;
	iconv_t cd;

	for (i = 0; i < sizeof cand / sizeof cand[0] && n < 2; i++) {
		for (j = 0; j < sizeof cand / sizeof cand[0]; j++) {
			if (i == j)
				continue;
			cd = iconv_open(cand[i], cand[j]);
			if (cd != (iconv_t)-1) {
				(void)iconv_close(cd);
				out[n++] = cand[i];
				break;
			}
		}
	}
	return n;
}

#endif
