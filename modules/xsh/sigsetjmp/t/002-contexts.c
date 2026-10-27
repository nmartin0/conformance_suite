/*
 * sigsetjmp() in each permitted context.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sigsetjmp.002
 */
#include "pc.h"
#include <setjmp.h>

static sigjmp_buf env;

int main(void)
{
	volatile int path;

	pc_begin("XSH.sigsetjmp.002");
	pc_test();

	path = 0;
	if (sigsetjmp(env, 1)) {
		path += 10;
	} else {
		path += 1;
		siglongjmp(env, 1);
	}
	pc_check(path == 11, "if (sigsetjmp()): path %d, expected 11", path);

	path = 0;
	switch (sigsetjmp(env, 0)) {
	case 0:
		path += 1;
		siglongjmp(env, 2);
	case 2:
		path += 10;
		break;
	default:
		path += 100;
		break;
	}
	pc_check(path == 11, "switch (sigsetjmp()): path %d, expected 11", path);

	path = 0;
	while (sigsetjmp(env, 1) == 0) {
		path += 1;
		siglongjmp(env, 1);
	}
	path += 10;
	pc_check(path == 11, "while (sigsetjmp() == 0): path %d, expected 11", path);

	path = 0;
	if (sigsetjmp(env, 0) != 0) {
		path += 10;
	} else {
		path += 1;
		siglongjmp(env, 5);
	}
	pc_check(path == 11, "if (sigsetjmp() != 0): path %d, expected 11", path);

	path = 0;
	if (!sigsetjmp(env, 1)) {
		path += 1;
		siglongjmp(env, 1);
	} else {
		path += 10;
	}
	pc_check(path == 11, "if (!sigsetjmp()): path %d, expected 11", path);

	path = 0;
	(void)sigsetjmp(env, 1);
	path++;
	if (path < 3)
		siglongjmp(env, 1);
	pc_check(path == 3, "(void)sigsetjmp() as a statement: path %d, expected 3", path);
	pc_pass();
}
