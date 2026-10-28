/*
 * <stdarg.h> va arg compatible
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stdarg.h.006
 *
 * Only <stdarg.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <stdarg.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)


/* Each reader takes the one variable argument as the type its name gives. */
#define READER(name, type) static type name(int dummy, ...) \
{ va_list ap; type v; va_start(ap, dummy); v = va_arg(ap, type); va_end(ap); return v; }

READER(rd_int, int)
READER(rd_uint, unsigned int)
READER(rd_long, long)
READER(rd_ulong, unsigned long)
READER(rd_llong, long long)
READER(rd_ullong, unsigned long long)
READER(rd_charp, char *)
READER(rd_voidp, void *)

int main(void)
{
	char c = 'x';

	pc_begin("XBD.stdarg.h.006");
	pc_test();
	/* a value representable in both a signed type and its unsigned counterpart reads back unchanged */
	if (rd_uint(0, 5) != 5u)
		pc_fail("an int argument 5 read as unsigned int was not 5");
	if (rd_int(0, 7u) != 7)
		pc_fail("an unsigned int argument 7 read as int was not 7");
	if (rd_ulong(0, 9L) != 9UL)
		pc_fail("a long argument 9 read as unsigned long was not 9");
	if (rd_long(0, 11UL) != 11L)
		pc_fail("an unsigned long argument 11 read as long was not 11");
	if (rd_ullong(0, 13LL) != 13ULL)
		pc_fail("a long long argument 13 read as unsigned long long was not 13");
	if (rd_llong(0, 15ULL) != 15LL)
		pc_fail("an unsigned long long argument 15 read as long long was not 15");
	/* a pointer to void and a pointer to a character type are interchangeable */
	if (rd_charp(0, (void *)&c) != &c)
		pc_fail("a void * argument read as char * was a different pointer");
	if (rd_voidp(0, (char *)&c) != (void *)&c)
		pc_fail("a char * argument read as void * was a different pointer");
	pc_pass();
}
