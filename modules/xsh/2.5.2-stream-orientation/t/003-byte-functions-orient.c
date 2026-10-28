/*
 * Each byte input/output function makes a new stream byte-oriented.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.5.2-stream-orientation.003
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <wchar.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

enum { PUTC, FPUTS, FWRITE, FPRINTF, PUTC2, GETC, FGETS, FREAD, FSCANF, GETC2, NOPS };
static const char *const opname[NOPS] = {
	"fputc", "fputs", "fwrite", "fprintf", "putc", "fgetc", "fgets", "fread", "fscanf", "getc"
};

int main(void)
{
	FILE *s[NOPS];
	int i;

	pc_begin("XSH.2.5.2-stream-orientation.003");
	pc_make_file("in", "abcdef");
	for (i = 0; i < NOPS; i++) {
		s[i] = fopen(i < GETC ? "out" : "in", i < GETC ? "w" : "r");
		NEED(s[i] != NULL, "fopen failed: %s", pc_errname(errno));
		NEED(fwide(s[i], 0) == 0, "a new stream is not without orientation");
	}
	pc_test();
	{
		char buf[8];
		char ch;
		int rc[NOPS];

		rc[PUTC] = fputc('x', s[PUTC]) == 'x';
		rc[FPUTS] = fputs("x", s[FPUTS]) >= 0;
		rc[FWRITE] = fwrite("x", 1, 1, s[FWRITE]) == 1;
		rc[FPRINTF] = fprintf(s[FPRINTF], "x") == 1;
		rc[PUTC2] = putc('x', s[PUTC2]) == 'x';
		rc[GETC] = fgetc(s[GETC]) == 'a';
		rc[FGETS] = fgets(buf, sizeof buf, s[FGETS]) != NULL;
		rc[FREAD] = fread(buf, 1, 2, s[FREAD]) == 2;
		rc[FSCANF] = fscanf(s[FSCANF], "%c", &ch) == 1;
		rc[GETC2] = getc(s[GETC2]) == 'a';
		for (i = 0; i < NOPS; i++) {
			int o;

			NEED(rc[i], "%s did not succeed", opname[i]);
			o = fwide(s[i], 0);
			pc_check(o < 0, "after %s fwide() returned %d, expected a negative value", opname[i], o);
		}
	}
	pc_pass();
}
