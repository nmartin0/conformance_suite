/*
 * An ISO C function whose prototype needs no header type can be declared
 * by the program and used without including its header.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.1-use-of-functions.004
 */
#include "pc.h"

/* No other header is included: pc.h brings only <stddef.h>. */
int abs(int);
long labs(long);
int atoi(const char *);
double atof(const char *);
int rand(void);
void srand(unsigned int);
int toupper(int);
int tolower(int);
int isalpha(int);
int isdigit(int);
int strcmp(const char *, const char *);
char *strcpy(char *, const char *);
int sprintf(char *, const char *, ...);

int main(void)
{
	char buf[32];
	int a, b;
	double d;

	pc_begin("XSH.2.1-use-of-functions.004");
	pc_test();
	pc_check(abs(-12) == 12, "abs(-12) is not 12");
	pc_check(labs(-12L) == 12L, "labs(-12) is not 12");
	pc_check(atoi("345") == 345, "atoi(\"345\") is not 345");
	d = atof("2.5");
	pc_check(d > 2.49 && d < 2.51, "atof(\"2.5\") is not 2.5");
	srand(7u);
	a = rand();
	srand(7u);
	b = rand();
	pc_check(a == b && a >= 0, "rand() after the same srand() gave %d and %d", a, b);
	pc_check(toupper('a') == 'A' && tolower('A') == 'a', "toupper or tolower is wrong");
	pc_check(isalpha('a') != 0 && isalpha('1') == 0, "isalpha is wrong");
	pc_check(isdigit('1') != 0 && isdigit('a') == 0, "isdigit is wrong");
	pc_check(strcpy(buf, "abc") == buf, "strcpy did not return its destination");
	pc_check(strcmp(buf, "abc") == 0 && strcmp(buf, "abd") < 0 && strcmp("b", buf) > 0, "strcmp is wrong");
	a = sprintf(buf, "%d-%s", 42, "x");
	pc_check(a == 4 && strcmp(buf, "42-x") == 0, "sprintf wrote the wrong text (%d)", a);
	pc_pass();
}
