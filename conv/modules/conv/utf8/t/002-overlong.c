/*
 * mbrtowc rejects overlong forms.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.utf8.002
 * MUTANT mbrtowc.overlong
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pc.h"
#include "../../../../lib/cv.h"
#include "../../../../lib/u8.h"

static void reject(const unsigned char *s, size_t n)
{
	wchar_t wc;
	size_t len, k;
	unsigned long cp;

	pc_check(u8_valid(s, n, &cp) == 0, "internal: sequence is valid UTF-8");
	if (u8_decodes(s, n, &wc, &len)) {
		char hex[64] = "";

		for (k = 0; k < n && k < 8; k++)
			(void)snprintf(hex + strlen(hex), 8, "%02X ", s[k]);
		pc_fail("overlong sequence %sis decoded (%zu bytes) instead of rejected", hex, len);
	}
}

int main(void)
{
	unsigned char s[8];
	unsigned a, b, c;

	pc_begin("CONV.utf8.002");
	u8_locale();
	pc_test();
	for (b = 0x80; b <= 0xBF; b++) {		/* C0 and C1 lead bytes */
		s[0] = 0xC0; s[1] = (unsigned char)b; reject(s, 2);
		s[0] = 0xC1; reject(s, 2);
	}
	for (a = 0x80; a <= 0x9F; a++)			/* E0 followed by less than A0 */
		for (b = 0x80; b <= 0xBF; b++) {
			s[0] = 0xE0; s[1] = (unsigned char)a; s[2] = (unsigned char)b;
			reject(s, 3);
		}
	for (a = 0x80; a <= 0x8F; a++)			/* F0 followed by less than 90 */
		for (b = 0x80; b <= 0xBF; b++)
			for (c = 0x80; c <= 0xBF; c += 0x3F) {
				s[0] = 0xF0; s[1] = (unsigned char)a; s[2] = (unsigned char)b; s[3] = (unsigned char)c;
				reject(s, 4);
			}
	/* the five- and six-octet forms of the original definition, carrying a value that fits in fewer octets */
	memcpy(s, "\xF8\x80\x80\x80\x80", 5); reject(s, 5);
	memcpy(s, "\xFC\x80\x80\x80\x80\x80", 6); reject(s, 6);
	/* the RFC's own example: the path separator written as C0 AF, and C0 AE for the dot */
	memcpy(s, "\xC0\xAE", 2); reject(s, 2);
	memcpy(s, "\xC0\xAF", 2); reject(s, 2);
	pc_pass();
}
