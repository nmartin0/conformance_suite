/*
 * Initializers are applied in list order, a later initializer for a
 * subobject replaces an earlier one, and the subobjects never named
 * are default-initialized.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.3-default-initialization.003
 */
#include "pc.h"

struct P { int x, y, z; };
struct N { struct P in; int w; };

static void dirty(void)
{
	volatile unsigned char buf[1024];
	unsigned i;

	for (i = 0; i < sizeof buf; i++)
		buf[i] = 0xA5;
}

static struct P sp = { .x = 1, .y = 2, .x = 3 };
static int sa[5] = { 1, 2, 3, [1] = 20, 40 };

static int autos(void)
{
	struct P p = { .x = 1, .y = 2, .x = 3 };
	struct P q = { 1, 2, 3, .y = 9 };
	struct P s = { .y = 5, 6 };
	int a[5] = { 1, 2, 3, [1] = 20, 40 };
	int b[4] = { [2] = 1, 5, [0] = 7, [2] = 9 };
	struct N n = { .in = { 1, 2 }, .w = 4, .in.z = 8 };
	struct P ps[2] = { [1].y = 4, [0].x = 6, [1].x = 7 };
	char t[4] = { 'x', [2] = 'z' };
	char str[6] = "abc";
	int ok = 1;

	ok &= p.x == 3 && p.y == 2 && p.z == 0;
	ok &= q.x == 1 && q.y == 9 && q.z == 3;
	ok &= s.x == 0 && s.y == 5 && s.z == 6;
	ok &= a[0] == 1 && a[1] == 20 && a[2] == 40 && a[3] == 0 && a[4] == 0;
	ok &= b[0] == 7 && b[1] == 0 && b[2] == 9 && b[3] == 5;
	ok &= n.in.x == 1 && n.in.y == 2 && n.in.z == 8 && n.w == 4;
	ok &= ps[0].x == 6 && ps[0].y == 0 && ps[0].z == 0 && ps[1].x == 7 && ps[1].y == 4 && ps[1].z == 0;
	ok &= t[0] == 'x' && t[1] == 0 && t[2] == 'z' && t[3] == 0;
	ok &= str[0] == 'a' && str[1] == 'b' && str[2] == 'c' && str[3] == 0 && str[4] == 0 && str[5] == 0;
	return ok;
}

int main(void)
{
	pc_begin("XBD.4.3-default-initialization.003");
	pc_test();
	pc_check(sp.x == 3 && sp.y == 2 && sp.z == 0, "static struct { .x = 1, .y = 2, .x = 3 } is {%d, %d, %d}", sp.x, sp.y, sp.z);
	pc_check(sa[0] == 1 && sa[1] == 20 && sa[2] == 40 && sa[3] == 0 && sa[4] == 0,
	    "static array { 1, 2, 3, [1] = 20, 40 } is {%d, %d, %d, %d, %d}", sa[0], sa[1], sa[2], sa[3], sa[4]);
	dirty();
	pc_check(autos(), "an automatic object with repeated or reordered designators does not hold the last initializer for each subobject");
	pc_pass();
}
