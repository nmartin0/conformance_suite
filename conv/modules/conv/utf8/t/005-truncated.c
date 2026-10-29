/*
 * mbrtowc does not decode truncated sequences.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.utf8.005
 * MUTANT mbrtowc.truncated
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pc.h"
#include "../../../../lib/cv.h"
#include "../../../../lib/u8.h"

static const unsigned long samples[] = { 0xE9, 0x7FF, 0x20AC, 0xFFFF, 0x10348, 0x10FFFF, 0 };

int main(void)
{
	unsigned char s[8];
	wchar_t wc;
	size_t len;
	int n, k, i;

	pc_begin("CONV.utf8.005");
	u8_locale();
	pc_test();
	for (i = 0; samples[i] != 0; i++) {
		n = u8_encode(samples[i], s);
		for (k = 1; k < n; k++) {
			/* the first k bytes alone */
			pc_check(!u8_decodes(s, (size_t)k, &wc, &len), "the first %d bytes of U+%04lX are decoded as a character", k, samples[i]);
			/* the first k bytes followed by a byte that is not a continuation */
			s[k] = 'A';
			pc_check(!u8_decodes(s, (size_t)k + 1, &wc, &len), "the first %d bytes of U+%04lX followed by A are decoded", k, samples[i]);
			(void)u8_encode(samples[i], s);	/* restore */
		}
	}
	pc_pass();
}
