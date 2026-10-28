/*
 * Operations that are not input or output leave an oriented stream oriented.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.5.2-stream-orientation.002
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <wchar.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* The same operations as in 001, on a stream that holds one character already. */
static int others(FILE *f, fpos_t *pos)
{
	if (feof(f) != 0 || ferror(f) != 0)
		return 1;
	clearerr(f);
	if (fileno(f) < 0)
		return 2;
	flockfile(f);
	funlockfile(f);
	if (fflush(f) != 0)
		return 3;
	if (ftell(f) != 1)
		return 4;
	if (fgetpos(f, pos) != 0)
		return 5;
	if (fseek(f, 0, SEEK_SET) != 0 || ftell(f) != 0)
		return 6;
	if (fsetpos(f, pos) != 0 || ftell(f) != 1)
		return 7;
	rewind(f);
	if (ftell(f) != 0)
		return 8;
	return 0;
}

int main(void)
{
	FILE *b, *w;
	fpos_t pb, pw;
	int r, o, c;
	wint_t wc;

	pc_begin("XSH.2.5.2-stream-orientation.002");
	b = fopen("bytes", "w+");
	w = fopen("wides", "w+");
	NEED(b != NULL && w != NULL, "fopen failed: %s", pc_errname(errno));
	pc_test();
	pc_check(fputc('a', b) == 'a', "fputc failed");
	pc_check(fputwc(L'a', w) == L'a', "fputwc failed");
	NEED(fwide(b, 0) < 0 && fwide(w, 0) > 0, "the streams are not oriented after output");
	r = others(b, &pb);
	pc_check(r == 0, "an operation on the byte-oriented stream failed or gave a wrong result (step %d)", r);
	r = others(w, &pw);
	pc_check(r == 0, "an operation on the wide-oriented stream failed or gave a wrong result (step %d)", r);
	o = fwide(b, 0);
	pc_check(o < 0, "the byte-oriented stream gives fwide() %d after those operations", o);
	o = fwide(w, 0);
	pc_check(o > 0, "the wide-oriented stream gives fwide() %d after those operations", o);
	c = fgetc(b);
	pc_check(c == 'a', "fgetc after rewind gave %d, not 'a'", c);
	wc = fgetwc(w);
	pc_check(wc == L'a', "fgetwc after rewind gave %lu, not L'a'", (unsigned long)wc);
	pc_pass();
}
