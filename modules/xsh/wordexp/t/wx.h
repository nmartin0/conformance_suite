/*
 * wx.h - helpers shared by the wordexp tests.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef WX_H
#define WX_H

#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wordexp.h>

#define WX_N(t) (sizeof(t) / sizeof((t)[0]))
#define WX_MAXW 8

/* One expansion: n words, v[0..n-1] (n == 0 means no words). */
struct wxc {
	const char *w;
	int fl;
	size_t n;
	const char *v[WX_MAXW];
};

/* Check the list: offs leading null pointers, n words, then a null pointer. */
static inline void wx_check(const wordexp_t *p, size_t offs, size_t n, const char *const *v,
    const char *what)
{
	size_t i;

	pc_check(p->we_wordc == n, "%s: we_wordc is %zu, expected %zu", what, (size_t)p->we_wordc, n);
	pc_check(p->we_wordv != NULL, "%s: we_wordv is null", what);
	for (i = 0; i < offs; i++)
		pc_check(p->we_wordv[i] == NULL, "%s: we_wordv[%zu] is not a null pointer", what, i);
	for (i = 0; i < n; i++) {
		pc_check(p->we_wordv[offs + i] != NULL, "%s: word %zu is null", what, i);
		pc_check(strcmp(p->we_wordv[offs + i], v[i]) == 0,
		    "%s: word %zu is \"%s\", expected \"%s\"", what, i, p->we_wordv[offs + i], v[i]);
	}
	pc_check(p->we_wordv[offs + n] == NULL, "%s: the list is not null-terminated", what);
}

static inline void wx_cases(const struct wxc *t, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++) {
		wordexp_t w;
		int r;

		memset(&w, 0, sizeof w);
		r = wordexp(t[i].w, &w, t[i].fl);
		pc_check(r == 0, "case %zu: wordexp(\"%s\") returned %d", i, t[i].w, r);
		wx_check(&w, 0, t[i].n, t[i].v, t[i].w);
		wordfree(&w);
	}
}

/* wordexp must fail with the code a or b; with zero set the word count must then be 0. */
static inline void wx_fails(const char *words, int fl, int a, int b, int zero)
{
	wordexp_t w;
	int r;

	memset(&w, 0, sizeof w);
	w.we_wordc = 99;
	r = wordexp(words, &w, fl);
	pc_check(r == a || r == b, "wordexp(\"%s\") returned %d, expected %d or %d", words, r, a, b);
	if (zero)
		pc_check(w.we_wordc == 0, "wordexp(\"%s\") failed but we_wordc is %zu", words,
		    (size_t)w.we_wordc);
}

#endif
