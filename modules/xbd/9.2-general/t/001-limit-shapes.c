/*
 * REs of exactly 256 bytes in several shapes are accepted and work.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.2-general.001
 */
#include "rx.h"

static char pat[300];

static void len256(void)
{
	if (strlen(pat) != 256)
		pc_unresolved("setup: pattern is %zu bytes long, not 256", strlen(pat));
}

static void rep(const char *s, int n)
{
	int i;

	for (i = 0; i < n; i++)
		strcat(pat, s);
}

static void nsub(int cf, size_t want)
{
	regex_t re;
	int r = regcomp(&re, pat, cf);

	pc_check(r == 0, "regcomp of the 256-byte pattern returned %d", r);
	pc_check(re.re_nsub == want, "re_nsub is %zu, expected %zu", (size_t)re.re_nsub, want);
	regfree(&re);
}

int main(void)
{
	static const char alpha[] = "abcdefghijklmnop";
	static char str[400];
	int i, cf;

	pc_begin("XBD.9.2-general.001");
	pc_test();

	/* a bracket expression of 254 list characters */
	for (cf = 0; cf <= REG_EXTENDED; cf += REG_EXTENDED) {
		pat[0] = '[';
		for (i = 0; i < 254; i++)
			pat[1 + i] = alpha[i % 16];
		pat[255] = ']';
		pat[256] = '\0';
		len256();
		rx_one(pat, cf, "zzp", 2, 3);
		rx_one(pat, cf, "zzz", -1, 0);
		rx_one(pat, cf, "a", 0, 1);
	}

	/* 84 alternatives and a literal run (ERE) */
	pat[0] = '\0';
	rep("ab|", 84);
	strcat(pat, "zzzz");
	len256();
	rx_one(pat, REG_EXTENDED, "zzzz", 0, 4);
	rx_one(pat, REG_EXTENDED, "xxab", 2, 4);
	rx_one(pat, REG_EXTENDED, "zzz", -1, 0);
	rx_one(pat, REG_EXTENDED, "xzzzzz", 1, 5);

	/* groups nested 63 deep (BRE) and 127 deep (ERE) */
	pat[0] = '\0';
	rep("\\(", 63);
	strcat(pat, "aaaa");
	rep("\\)", 63);
	len256();
	nsub(0, 63);
	rx_one(pat, 0, "baaaab", 1, 5);
	rx_one(pat, 0, "aaa", -1, 0);
	pat[0] = '\0';
	rep("(", 127);
	strcat(pat, "aa");
	rep(")", 127);
	len256();
	nsub(REG_EXTENDED, 127);
	rx_one(pat, REG_EXTENDED, "baab", 1, 3);
	rx_one(pat, REG_EXTENDED, "a", -1, 0);

	/* intervals: 28 in a BRE, 36 and a literal run in an ERE (each followed by
	   a delimiter, so that the match needs no search among many splits) */
	pat[0] = '\0';
	rep("a\\{1,2\\}b", 28);
	strcat(pat, "cccc");
	len256();
	strcpy(str, "aab");
	for (i = 1; i < 28; i++)
		strcat(str, "ab");
	strcat(str, "cccc");
	rx_one(pat, 0, str, 0, 61);
	strcpy(str, "ab");
	for (i = 1; i < 28; i++)
		strcat(str, "ab");
	strcat(str, "cccc");
	rx_one(pat, 0, str, 0, 60);
	strcpy(str, "ab");
	for (i = 1; i < 27; i++)
		strcat(str, "ab");
	strcat(str, "cccc");
	rx_one(pat, 0, str, -1, 0);
	pat[0] = '\0';
	rep("a{1,2}b", 36);
	strcat(pat, "cccc");
	len256();
	strcpy(str, "aab");
	for (i = 1; i < 36; i++)
		strcat(str, "ab");
	strcat(str, "cccc");
	rx_one(pat, REG_EXTENDED, str, 0, 77);
	strcpy(str, "ab");
	for (i = 1; i < 35; i++)
		strcat(str, "ab");
	strcat(str, "cccc");
	rx_one(pat, REG_EXTENDED, str, -1, 0);

	/* 128 escaped periods and 128 starred items */
	for (cf = 0; cf <= REG_EXTENDED; cf += REG_EXTENDED) {
		pat[0] = '\0';
		rep("\\.", 128);
		len256();
		memset(str, '.', 128);
		str[128] = '\0';
		rx_one(pat, cf, str, 0, 128);
		str[127] = '\0';
		rx_one(pat, cf, str, -1, 0);
		pat[0] = '\0';
		rep("a*", 128);
		len256();
		rx_one(pat, cf, "xaaa", 0, 0);
		rx_one(pat, cf, "aaa", 0, 3);
	}
	pc_pass();
}
