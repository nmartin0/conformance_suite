/*
 * After #undef of a name, the name refers to an actual function.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.1-use-of-functions.002
 */
#include "pc.h"
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* The #undef directives follow every #include, as 2.2.2 asks. */
#undef getc
#undef putc
#undef getchar
#undef putchar
#undef feof
#undef ferror
#undef clearerr
#undef fileno
#undef getc_unlocked
#undef putc_unlocked
#undef getchar_unlocked
#undef putchar_unlocked
#undef isalpha
#undef isdigit
#undef toupper
#undef tolower
#undef abs
#undef labs

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

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
	int (*p_toupper)(int) = toupper;
	int (*p_getc)(FILE *) = getc;
	int (*p_abs)(int) = abs;

	pc_begin("XSH.2.1-use-of-functions.002");
	pc_make_file("in", "abcdefghijkl");
	pc_make_file("out1", "");
	r = fopen("in", "r");
	w = fopen("out1", "w");
	NEED(r != NULL && w != NULL, "fopen failed");
	pc_test();
	pc_check(getc(r) == 'a' && p_getc(r) == 'b' && getc_unlocked(r) == 'c', "getc after #undef failed");
	pc_check(feof(r) == 0 && ferror(r) == 0 && fileno(r) >= 0, "feof, ferror or fileno failed after #undef");
	clearerr(r);
	pc_check(putc('X', w) == 'X' && putc_unlocked('Y', w) == 'Y', "putc after #undef failed");
	pc_check(fclose(w) == 0 && pc_file_equals("out1", "XY"), "the stream output is not XY");

	saved = redirect(0, "in", O_RDONLY);
	c = getchar();
	if (c == 'a')
		c = getchar_unlocked();
	restore(0, saved);
	clearerr(stdin);
	pc_check(c == 'b', "getchar after #undef failed");

	pc_make_file("out2", "");
	saved = redirect(1, "out2", O_WRONLY | O_TRUNC);
	c = putchar('1');
	if (c == '1')
		c = putchar_unlocked('2');
	(void)fflush(stdout);
	restore(1, saved);
	pc_check(c == '2' && pc_file_equals("out2", "12"), "putchar after #undef failed");

	pc_check(isalpha('q') != 0 && isalpha('5') == 0, "isalpha after #undef is wrong");
	pc_check(isdigit('5') != 0 && isdigit('q') == 0, "isdigit after #undef is wrong");
	pc_check(toupper('q') == 'Q' && p_toupper('q') == 'Q', "toupper after #undef is wrong");
	pc_check(tolower('Q') == 'q', "tolower after #undef is wrong");
	pc_check(abs(-9) == 9 && p_abs(-9) == 9 && labs(-9L) == 9L, "abs or labs after #undef is wrong");
	pc_pass();
}
