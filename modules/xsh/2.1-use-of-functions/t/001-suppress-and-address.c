/*
 * A function name in parentheses and a function's address work whether or
 * not the header also defines a macro of that name.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.1-use-of-functions.001
 */
#include "pc.h"
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* Move descriptor 'to' onto file 'path' opened with 'flags'; return a copy of the old one. */
static int redirect(int to, const char *path, int flags)
{
	int saved = dup(to), fd = open(path, flags, 0600);

	NEED(saved >= 0 && fd >= 0, "cannot redirect descriptor %d", to);
	NEED(dup2(fd, to) == to, "dup2 failed");
	(void)close(fd);
	return saved;
}

static void restore(int to, int saved)
{
	NEED(dup2(saved, to) == to, "dup2 (restore) failed");
	(void)close(saved);
}

int main(void)
{
	FILE *r, *w;
	int saved, c;
	int (*p_getc)(FILE *) = getc;
	int (*p_putc)(int, FILE *) = putc;
	int (*p_getchar)(void) = getchar;
	int (*p_putchar)(int) = putchar;
	int (*p_feof)(FILE *) = feof;
	int (*p_ferror)(FILE *) = ferror;
	void (*p_clearerr)(FILE *) = clearerr;
	int (*p_fileno)(FILE *) = fileno;
	int (*p_getc_u)(FILE *) = getc_unlocked;
	int (*p_putc_u)(int, FILE *) = putc_unlocked;
	int (*p_getchar_u)(void) = getchar_unlocked;
	int (*p_putchar_u)(int) = putchar_unlocked;
	int (*p_isalpha)(int) = isalpha;
	int (*p_isdigit)(int) = isdigit;
	int (*p_toupper)(int) = toupper;
	int (*p_tolower)(int) = tolower;
	int (*p_abs)(int) = abs;
	long (*p_labs)(long) = labs;

	pc_begin("XSH.2.1-use-of-functions.001");
	pc_make_file("in", "abcdefghijkl");
	pc_make_file("out1", "");
	r = fopen("in", "r");
	w = fopen("out1", "w");
	NEED(r != NULL && w != NULL, "fopen failed");
	pc_test();

	/* stream functions: name in parentheses, then address */
	c = (getc)(r);
	pc_check(c == 'a', "(getc)(r) returned %d", c);
	c = p_getc(r);
	pc_check(c == 'b', "getc through its address returned %d", c);
	c = (getc_unlocked)(r);
	pc_check(c == 'c', "(getc_unlocked)(r) returned %d", c);
	c = p_getc_u(r);
	pc_check(c == 'd', "getc_unlocked through its address returned %d", c);
	pc_check((feof)(r) == 0 && p_feof(r) == 0, "feof reported end-of-file too early");
	pc_check((ferror)(r) == 0 && p_ferror(r) == 0, "ferror reported an error");
	pc_check((fileno)(r) >= 0 && (fileno)(r) == p_fileno(r), "fileno disagrees with itself");
	(clearerr)(r);
	p_clearerr(r);
	pc_check((putc)('X', w) == 'X', "(putc) failed");
	pc_check(p_putc('Y', w) == 'Y', "putc through its address failed");
	pc_check((putc_unlocked)('Z', w) == 'Z', "(putc_unlocked) failed");
	pc_check(p_putc_u('W', w) == 'W', "putc_unlocked through its address failed");
	pc_check(fclose(w) == 0, "fclose failed");
	pc_check(pc_file_equals("out1", "XYZW"), "the stream output is not XYZW");

	/* standard input and output */
	saved = redirect(0, "in", O_RDONLY);
	c = (getchar)();
	pc_check(c == 'a', "(getchar)() returned %d", c);
	c = p_getchar();
	pc_check(c == 'b', "getchar through its address returned %d", c);
	c = (getchar_unlocked)();
	pc_check(c == 'c', "(getchar_unlocked)() returned %d", c);
	c = p_getchar_u();
	pc_check(c == 'd', "getchar_unlocked through its address returned %d", c);
	restore(0, saved);
	clearerr(stdin);

	pc_make_file("out2", "");
	saved = redirect(1, "out2", O_WRONLY | O_TRUNC);
	c = (putchar)('1');
	if (c == '1')
		c = p_putchar('2');
	if (c == '2')
		c = (putchar_unlocked)('3');
	if (c == '3')
		c = p_putchar_u('4');
	(void)fflush(stdout);
	restore(1, saved);
	pc_check(c == '4', "a putchar form failed");
	pc_check(pc_file_equals("out2", "1234"), "the standard output text is not 1234");

	/* character classes and conversions */
	pc_check((isalpha)('q') != 0 && p_isalpha('q') != 0, "isalpha('q') is false");
	pc_check((isalpha)('5') == 0 && p_isalpha('5') == 0, "isalpha('5') is true");
	pc_check((isdigit)('5') != 0 && p_isdigit('5') != 0, "isdigit('5') is false");
	pc_check((isdigit)('q') == 0 && p_isdigit('q') == 0, "isdigit('q') is true");
	pc_check((toupper)('q') == 'Q' && p_toupper('q') == 'Q', "toupper('q') is not 'Q'");
	pc_check((tolower)('Q') == 'q' && p_tolower('Q') == 'q', "tolower('Q') is not 'q'");
	pc_check((abs)(-7) == 7 && p_abs(-7) == 7, "abs(-7) is not 7");
	pc_check((labs)(-7L) == 7L && p_labs(-7L) == 7L, "labs(-7) is not 7");
	pc_pass();
}
