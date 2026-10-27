/*
 * setjmp() in each permitted context.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setjmp.002
 */
#include "pc.h"
#include <setjmp.h>

static jmp_buf env;

int main(void)
{
	volatile int path;

	pc_begin("XSH.setjmp.002");
	pc_test();

	path = 0;
	if (setjmp(env)) {
		path += 10;
	} else {
		path += 1;
		longjmp(env, 1);
	}
	pc_check(path == 11, "if (setjmp()): path %d, expected 11", path);

	path = 0;
	switch (setjmp(env)) {
	case 0:
		path += 1;
		longjmp(env, 2);
	case 2:
		path += 10;
		break;
	default:
		path += 100;
		break;
	}
	pc_check(path == 11, "switch (setjmp()): path %d, expected 11", path);

	path = 0;
	while (setjmp(env) == 0) {
		path += 1;
		longjmp(env, 1);
	}
	path += 10;
	pc_check(path == 11, "while (setjmp() == 0): path %d, expected 11", path);

	path = 0;
	if (setjmp(env) != 0) {
		path += 10;
	} else {
		path += 1;
		longjmp(env, 5);
	}
	pc_check(path == 11, "if (setjmp() != 0): path %d, expected 11", path);

	path = 0;
	if (!setjmp(env)) {
		path += 1;
		longjmp(env, 1);
	} else {
		path += 10;
	}
	pc_check(path == 11, "if (!setjmp()): path %d, expected 11", path);

	path = 0;
	(void)setjmp(env);
	path++;
	if (path < 3)
		longjmp(env, 1);
	pc_check(path == 3, "(void)setjmp() as a statement: path %d, expected 3", path);

	path = 0;
	setjmp(env);
	path++;
	if (path < 3)
		longjmp(env, 1);
	pc_check(path == 3, "setjmp() as a statement: path %d, expected 3", path);
	pc_pass();
}
