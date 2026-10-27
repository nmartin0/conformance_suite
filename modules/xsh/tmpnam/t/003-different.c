/*
 * Successive tmpnam() results differ.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tmpnam.003
 */
#include "pc.h"
#include <stdio.h>
#include <string.h>

#define N 25

int main(void)
{
	char names[N][L_tmpnam + 16];
	int i, j;

	pc_begin("XSH.tmpnam.003");
	pc_test();
	pc_check(TMP_MAX >= 25, "TMP_MAX is %d, below 25", (int)TMP_MAX);
	for (i = 0; i < N; i++) {
		char *r = (i & 1) ? tmpnam(NULL) : tmpnam(names[i]);

		pc_check(r != NULL, "call %d returned a null pointer", i);
		if (r != names[i])
			strncpy(names[i], r, sizeof names[i] - 1);
		names[i][sizeof names[i] - 1] = '\0';
	}
	for (i = 0; i < N; i++)
		for (j = i + 1; j < N; j++)
			pc_check(strcmp(names[i], names[j]) != 0, "calls %d and %d returned the same name \"%s\"", i, j, names[i]);
	pc_pass();
}
