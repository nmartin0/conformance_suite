/*
 * u8.h - the UTF-8 of RFC 3629 as an independent reference, for conv/utf8.
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Include after "pc.h". The reference is the RFC's own table (section 3)
 * and grammar (section 4); it does not call the C library.
 */
#ifndef U8_H
#define U8_H

#include <locale.h>
#include <langinfo.h>
#include <wchar.h>
#include <errno.h>

/* The RFC's encoding of cp (section 3), or 0 for a number it prohibits. */
static int u8_encode(unsigned long cp, unsigned char *o)
{
	if (cp >= 0xD800 && cp <= 0xDFFF)
		return 0;
	if (cp < 0x80) {
		o[0] = (unsigned char)cp;
		return 1;
	}
	if (cp < 0x800) {
		o[0] = (unsigned char)(0xC0 | (cp >> 6));
		o[1] = (unsigned char)(0x80 | (cp & 0x3F));
		return 2;
	}
	if (cp < 0x10000) {
		o[0] = (unsigned char)(0xE0 | (cp >> 12));
		o[1] = (unsigned char)(0x80 | ((cp >> 6) & 0x3F));
		o[2] = (unsigned char)(0x80 | (cp & 0x3F));
		return 3;
	}
	if (cp <= 0x10FFFF) {
		o[0] = (unsigned char)(0xF0 | (cp >> 18));
		o[1] = (unsigned char)(0x80 | ((cp >> 12) & 0x3F));
		o[2] = (unsigned char)(0x80 | ((cp >> 6) & 0x3F));
		o[3] = (unsigned char)(0x80 | (cp & 0x3F));
		return 4;
	}
	return 0;
}

#define U8_TAIL(c) ((c) >= 0x80 && (c) <= 0xBF)

/*
 * Does s (n bytes) begin with one UTF8-char of the ABNF in section 4?
 * Returns its length, or 0 if the bytes cannot begin one. *cp is the number.
 */
static int u8_valid(const unsigned char *s, size_t n, unsigned long *cp)
{
	unsigned c = n > 0 ? s[0] : 0;

	if (n == 0)
		return 0;
	if (c <= 0x7F) {
		*cp = c;
		return 1;
	}
	if (c >= 0xC2 && c <= 0xDF) {
		if (n < 2 || !U8_TAIL(s[1]))
			return 0;
		*cp = ((c & 0x1Fu) << 6) | (s[1] & 0x3Fu);
		return 2;
	}
	if (c >= 0xE0 && c <= 0xEF) {
		unsigned lo = 0x80, hi = 0xBF;

		if (c == 0xE0)
			lo = 0xA0;
		if (c == 0xED)
			hi = 0x9F;
		if (n < 3 || s[1] < lo || s[1] > hi || !U8_TAIL(s[2]))
			return 0;
		*cp = ((c & 0x0Fu) << 12) | ((s[1] & 0x3Fu) << 6) | (s[2] & 0x3Fu);
		return 3;
	}
	if (c >= 0xF0 && c <= 0xF4) {
		unsigned lo = 0x80, hi = 0xBF;

		if (c == 0xF0)
			lo = 0x90;
		if (c == 0xF4)
			hi = 0x8F;
		if (n < 4 || s[1] < lo || s[1] > hi || !U8_TAIL(s[2]) || !U8_TAIL(s[3]))
			return 0;
		*cp = ((c & 0x07ul) << 18) | ((s[1] & 0x3Ful) << 12) | ((s[2] & 0x3Ful) << 6) | (s[3] & 0x3Ful);
		return 4;
	}
	return 0;
}

/*
 * Select a UTF-8 locale (NOTINUSE if the system has none) and check that
 * its code set is UTF-8 with at most four bytes per character.
 */
static void u8_locale(void)
{
	const char *name = pc_find_locale("UTF-8", 1);

	if (name == NULL)
		pc_notinuse("the system has no installed locale with the UTF-8 code set");
	pc_check(setlocale(LC_ALL, name) != NULL, "cannot select locale %s", name);
	pc_check(MB_CUR_MAX >= 4, "MB_CUR_MAX is %d in locale %s: not able to hold four-byte UTF-8", (int)MB_CUR_MAX, name);
}

/* mbrtowc on exactly the n bytes of s with a fresh state; true if it decoded a character. */
static int u8_decodes(const unsigned char *s, size_t n, wchar_t *wc, size_t *len)
{
	mbstate_t ps;
	size_t r;

	memset(&ps, 0, sizeof ps);
	errno = 0;
	r = mbrtowc(wc, (const char *)s, n, &ps);
	*len = r;
	return r != (size_t)-1 && r != (size_t)-2;
}

#endif
