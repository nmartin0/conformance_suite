/*
 * getentropy() fills the buffer with unpredictable bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getentropy.003
 */
#include "pc.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	unsigned char a[256], b[32], c[32];
	int i, zeros = 0, round;

	pc_begin("XSH.getentropy.003");
	pc_test();
	for (round = 0; round < 2; round++) {
		int r;

		memset(a, round ? 0xFF : 0x00, sizeof a);
		r = getentropy(a, sizeof a);
		if (r == -1 && errno == ENOSYS)
			pc_notinuse("getentropy() is not provided (ENOSYS)");
		pc_check(r == 0, "getentropy(256) returned %d (errno %s)", r, pc_errname(errno));
		zeros = 0;
		for (i = 0; i < 256; i++)
			if (a[i] == (round ? 0xFF : 0x00))
				zeros++;
		pc_check(zeros < 16, "%d of 256 bytes still hold the fill value %s", zeros, round ? "0xFF" : "0x00");
	}
	pc_check(getentropy(b, sizeof b) == 0, "getentropy(32) failed");
	pc_check(getentropy(c, sizeof c) == 0, "getentropy(32) failed");
	pc_check(memcmp(b, c, sizeof b) != 0, "two 32-byte draws are identical");
	pc_pass();
}
