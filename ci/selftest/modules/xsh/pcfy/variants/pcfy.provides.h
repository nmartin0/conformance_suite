/*
 * pcfy.provides.h - fixture header variant: supplies pcfy_answer(), which
 * applies the fault "pcfy.wrong" itself (CI only).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#include <stdlib.h>
#include <string.h>
static int pcfy_answer(void)
{
	const char *ph = getenv("PC_PHASE"), *m = getenv("PC_MUTANT");

	if (ph != NULL && strcmp(ph, "test") == 0 && m != NULL && strcmp(m, "pcfy.wrong") == 0)
		return 41;
	return 42;
}
