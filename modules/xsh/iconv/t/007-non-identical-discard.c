/*
 * //NON_IDENTICAL_DISCARD drops a character with no identical counterpart but counts it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iconv.007
 */
#include "iv.h"

int main(void)
{
	/* 'a', U+00E9 in UTF-8, 'b': the middle one has no identical character in US-ASCII */
	char in[] = { 'a', (char)0xC3, (char)0xA9, 'b' };
	char out[16];
	iconv_t cd;
	struct iv_res r;

	pc_begin("XSH.iconv.007");
	cd = iv_open(iv_ascii, iv_utf8, "//NON_IDENTICAL_DISCARD", NULL, NULL);
	iv_need(cd);
	pc_test();
	r = iv_call(cd, in, sizeof in, out, sizeof out);
	pc_check(r.ret != (size_t)-1, "failed: %s", pc_errname(r.err));
	pc_check(r.ret == 1, "return value %lu, expected 1 (the discarded character is counted)", (unsigned long)r.ret);
	pc_check(r.inleft == 0, "input left %lu, expected 0", (unsigned long)r.inleft);
	pc_check(r.outp == out + 2 && memcmp(out, "ab", 2) == 0, "output is not \"ab\"");
	(void)iconv_close(cd);
	pc_pass();
}
