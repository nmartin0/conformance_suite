/*
 * jmp_buf and sigjmp_buf are array types.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.setjmp.h.001
 */
#include <setjmp.h>
#include "pc.h"


/* An array-typed parameter is adjusted to a pointer: the call below only compiles for an array type. */
static int same(jmp_buf p, void *want)
{
	return (void *)p == want;
}

static int same_sig(sigjmp_buf p, void *want)
{
	return (void *)p == want;
}

int main(void)
{
	jmp_buf jb;
	sigjmp_buf sb;

	pc_begin("XBD.setjmp.h.001");
	pc_test();
	pc_check(sizeof jb > 0 && (void *)&jb == (void *)jb, "jmp_buf is not an array type");
	pc_check(sizeof sb > 0 && (void *)&sb == (void *)sb, "sigjmp_buf is not an array type");
	pc_check(same(jb, (void *)jb), "a jmp_buf argument did not arrive as its own address");
	pc_check(same_sig(sb, (void *)sb), "a sigjmp_buf argument did not arrive as its own address");
	pc_pass();
}
