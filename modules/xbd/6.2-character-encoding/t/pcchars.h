/*
 * pcchars.h - the portable character set written as C character constants
 * (so no encoding is assumed), the classes the POSIX locale puts each
 * member in, and the symbolic names used in charmap files and locale
 * sources.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef PCCHARS_H
#define PCCHARS_H

#include <stdio.h>
#include <string.h>

static const char pcc_upper[]  = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char pcc_lower[]  = "abcdefghijklmnopqrstuvwxyz";
static const char pcc_digit[]  = "0123456789";
static const char pcc_xup[]    = "ABCDEF";
static const char pcc_xlow[]   = "abcdef";
static const char pcc_punct[]  = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
static const char pcc_space6[] = " \t\n\v\f\r";
static const char pcc_ctl7[]   = "\a\b\t\n\v\f\r";

#define PCC_UPPER  0x001u
#define PCC_LOWER  0x002u
#define PCC_DIGIT  0x004u
#define PCC_XDIGIT 0x008u
#define PCC_SPACE  0x010u
#define PCC_BLANK  0x020u
#define PCC_CNTRL  0x040u
#define PCC_PUNCT  0x080u
#define PCC_PORT   0x100u	/* member of the portable character set */

/* all 103 members: NUL, the seven controls, space, punctuation, digits, letters */
#define PCC_N 103
static char pcc_all[PCC_N + 1];
static unsigned pcc_flags[256];	/* what the POSIX locale makes of each portable member */

static void pcc_mark(const char *set, unsigned f)
{
	const char *p;

	for (p = set; *p != '\0'; p++)
		pcc_flags[(unsigned char)*p] |= f | PCC_PORT;
}

static void pcc_init(void)
{
	size_t n = 0;
	const char *p;

	memset(pcc_flags, 0, sizeof pcc_flags);
	pcc_mark(pcc_upper, PCC_UPPER);
	pcc_mark(pcc_lower, PCC_LOWER);
	pcc_mark(pcc_digit, PCC_DIGIT | PCC_XDIGIT);
	pcc_mark(pcc_xup, PCC_XDIGIT);
	pcc_mark(pcc_xlow, PCC_XDIGIT);
	pcc_mark(pcc_space6, PCC_SPACE);
	pcc_mark(" \t", PCC_BLANK);
	pcc_mark(pcc_ctl7, PCC_CNTRL);
	pcc_mark(pcc_punct, PCC_PUNCT);
	pcc_flags[0] |= PCC_CNTRL | PCC_PORT;
	pcc_flags[(unsigned char)' '] |= PCC_PORT;
	pcc_all[n++] = '\0';
	for (p = pcc_ctl7; *p != '\0'; p++)
		pcc_all[n++] = *p;
	pcc_all[n++] = ' ';
	for (p = pcc_punct; *p != '\0'; p++)
		pcc_all[n++] = *p;
	for (p = pcc_digit; *p != '\0'; p++)
		pcc_all[n++] = *p;
	for (p = pcc_upper; *p != '\0'; p++)
		pcc_all[n++] = *p;
	for (p = pcc_lower; *p != '\0'; p++)
		pcc_all[n++] = *p;
	pcc_all[n] = '\0';
}

/* Symbolic name (without the angle brackets) of a portable member. */
static const char *pcc_sym(char c, char *buf)
{
	static const char *const dn[] = { "zero", "one", "two", "three", "four",
	    "five", "six", "seven", "eight", "nine" };
	const char *d;

	switch (c) {
	case '\0': return "NUL";
	case '\a': return "alert";
	case '\b': return "backspace";
	case '\t': return "tab";
	case '\n': return "newline";
	case '\v': return "vertical-tab";
	case '\f': return "form-feed";
	case '\r': return "carriage-return";
	case ' ': return "space";
	case '!': return "exclamation-mark";
	case '"': return "quotation-mark";
	case '#': return "number-sign";
	case '$': return "dollar-sign";
	case '%': return "percent-sign";
	case '&': return "ampersand";
	case '\'': return "apostrophe";
	case '(': return "left-parenthesis";
	case ')': return "right-parenthesis";
	case '*': return "asterisk";
	case '+': return "plus-sign";
	case ',': return "comma";
	case '-': return "hyphen-minus";
	case '.': return "period";
	case '/': return "slash";
	case ':': return "colon";
	case ';': return "semicolon";
	case '<': return "less-than-sign";
	case '=': return "equals-sign";
	case '>': return "greater-than-sign";
	case '?': return "question-mark";
	case '@': return "commercial-at";
	case '[': return "left-square-bracket";
	case '\\': return "backslash";
	case ']': return "right-square-bracket";
	case '^': return "circumflex";
	case '_': return "underscore";
	case '`': return "grave-accent";
	case '{': return "left-curly-bracket";
	case '|': return "vertical-line";
	case '}': return "right-curly-bracket";
	case '~': return "tilde";
	default: break;
	}
	d = strchr(pcc_digit, c);
	if (d != NULL && c != '\0')
		return dn[d - pcc_digit];
	buf[0] = c;
	buf[1] = '\0';
	return buf;
}

/* The second name the table gives a member, or NULL. */
static const char *pcc_alias(char c)
{
	switch (c) {
	case '\a': return "BEL";
	case '\b': return "BS";
	case '\t': return "HT";
	case '\n': return "LF";
	case '\v': return "VT";
	case '\f': return "FF";
	case '\r': return "CR";
	case '-': return "hyphen";
	case '.': return "full-stop";
	case '/': return "solidus";
	case '\\': return "reverse-solidus";
	case '^': return "circumflex-accent";
	case '_': return "low-line";
	case '{': return "left-brace";
	case '}': return "right-brace";
	default: return NULL;
	}
}

/* Write "<a>;<b>;..." for the characters of set; returns out. */
static char *pcc_list(char *out, size_t n, const char *set)
{
	char nb[4];
	size_t len = 0;
	const char *p;
	int w;

	out[0] = '\0';
	for (p = set; *p != '\0'; p++) {
		w = snprintf(out + len, n - len, "%s<%s>", p == set ? "" : ";",
		    pcc_sym(*p, nb));
		if (w < 0 || (size_t)w >= n - len)
			break;
		len += (size_t)w;
	}
	return out;
}

/* The printable members in the order of the POSIX collation sequence (after the controls). */
static const char pcc_order[] = "\a\b\t\n\v\f\r !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

/* Write a string as locale-source text: "<S><u><n>" for "Sun" (with the quotes). */
static char *pcc_str(char *out, size_t n, const char *s)
{
	char nb[4];
	size_t len = 0;
	int w;

	w = snprintf(out, n, "\"");
	len = (size_t)w;
	for (; *s != '\0' && len < n; s++) {
		w = snprintf(out + len, n - len, "<%s>", pcc_sym(*s, nb));
		if (w < 0 || (size_t)w >= n - len)
			break;
		len += (size_t)w;
	}
	if (len + 2 <= n)
		(void)snprintf(out + len, n - len, "\"");
	return out;
}

/* Write "(<a>,<A>);(<b>,<B>);..." pairing the characters of from with those of to. */
static char *pcc_pairs(char *out, size_t n, const char *from, const char *to)
{
	char n1[4], n2[4];
	size_t len = 0;
	int w;

	out[0] = '\0';
	for (; *from != '\0' && *to != '\0'; from++, to++) {
		w = snprintf(out + len, n - len, "%s(<%s>,<%s>)", len == 0 ? "" : ";", pcc_sym(*from, n1),
		    pcc_sym(*to, n2));
		if (w < 0 || (size_t)w >= n - len)
			break;
		len += (size_t)w;
	}
	return out;
}

#endif
