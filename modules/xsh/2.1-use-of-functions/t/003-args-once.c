/*
 * Calls evaluate each argument exactly once and take compound arguments as a whole.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.1-use-of-functions.003
 */
#include "pc.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

int main(void)
{
	FILE *fs[4], *w[4];
	int i, j, k, c, n, flag = 1, other = 0;
	const char *s = "aB5 ";
	const char *q;
	long ln, lm;

	pc_begin("XSH.2.1-use-of-functions.003");
	pc_make_file("in", "abcdefgh");
	pc_make_file("o0", "");
	pc_make_file("o1", "");
	pc_make_file("o2", "");
	pc_make_file("o3", "");
	for (i = 0; i < 4; i++) {
		char nm[4];

		fs[i] = fopen("in", "r");
		nm[0] = 'o';
		nm[1] = (char)('0' + i);
		nm[2] = '\0';
		w[i] = fopen(nm, "w");
		NEED(fs[i] != NULL && w[i] != NULL, "fopen failed");
	}
	pc_test();

	/* getc and putc (and the unlocked forms) may evaluate their stream
	 * argument more than once; only the byte argument of putc counts */
	c = getc(fs[0]);
	pc_check(c == 'a', "getc returned %d", c);
	c = getc_unlocked(fs[1]);
	pc_check(c == 'a', "getc_unlocked returned %d", c);
	c = 'p';
	k = putc(c++, w[0]);
	pc_check(c == 'q' && k == 'p', "putc evaluated its byte argument other than once (c %d)", c);
	c = 'r';
	k = putc_unlocked(c++, w[1]);
	pc_check(c == 's' && k == 'r', "putc_unlocked evaluated its byte argument other than once");

	/* feof, ferror, clearerr, fileno: the stream argument */
	i = 0;
	j = feof(fs[i++]);
	pc_check(i == 1 && j == 0, "feof evaluated its argument %d times", i);
	i = 0;
	j = ferror(fs[i++]);
	pc_check(i == 1 && j == 0, "ferror evaluated its argument %d times", i);
	i = 0;
	clearerr(fs[i++]);
	pc_check(i == 1, "clearerr evaluated its argument %d times", i);
	i = 0;
	j = fileno(fs[i++]);
	pc_check(i == 1 && j >= 0, "fileno evaluated its argument %d times", i);

	/* ctype and conversion functions */
	q = s;
	n = isalpha((unsigned char)*q++);
	pc_check(q == s + 1 && n != 0, "isalpha evaluated its argument %d times", (int)(q - s));
	q = s + 2;
	n = isdigit((unsigned char)*q++);
	pc_check(q == s + 3 && n != 0, "isdigit evaluated its argument %d times", (int)(q - s) - 2);
	q = s;
	n = toupper((unsigned char)*q++);
	pc_check(q == s + 1 && n == 'A', "toupper evaluated its argument %d times", (int)(q - s));
	q = s + 1;
	n = tolower((unsigned char)*q++);
	pc_check(q == s + 2 && n == 'b', "tolower evaluated its argument %d times", (int)(q - s) - 1);
	k = -5;
	n = abs(k++);
	pc_check(k == -4 && n == 5, "abs evaluated its argument other than once");
	lm = -6;
	ln = labs(lm--);
	pc_check(lm == -7 && ln == 6, "labs evaluated its argument other than once (%ld, %ld)", lm, ln);

	/* arguments with operators of low precedence are taken as a whole */
	n = toupper(flag ? 'x' : 'y');
	pc_check(n == 'X', "toupper(flag ? 'x' : 'y') gave %d", n);
	n = tolower(other ? 'Q' : 'R');
	pc_check(n == 'r', "tolower(other ? 'Q' : 'R') gave %d", n);
	n = isdigit(c = '7');
	pc_check(n != 0 && c == '7', "isdigit(c = '7') gave %d", n);
	n = isalpha((flag++, 'z'));
	pc_check(n != 0 && flag == 2, "isalpha((flag++, 'z')) evaluated the comma expression %d times", flag - 1);
	n = abs(other ? -1 : -3);
	pc_check(n == 3, "abs(other ? -1 : -3) gave %d", n);
	k = putc(c = 'm', w[2]);
	pc_check(k == 'm' && c == 'm', "putc(c = 'm', stream) gave %d", k);
	k = putc((flag++, 'n'), w[3]);
	pc_check(k == 'n' && flag == 3, "putc((flag++, 'n'), stream) evaluated the byte argument other than once");
	pc_pass();
}
