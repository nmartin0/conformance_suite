/*
 * The random devices give readable, varying bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.devnodes.004
 */
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pc.h"
#include "../../../../lib/cv.h"

int main(void)
{
	static const char *const names[] = { "urandom", "random", NULL };
	char p[1024];
	struct stat st;
	unsigned char a[32], b[32];
	int i, fd, present = 0, j, same;
	ssize_t r;

	pc_begin("CONV.devnodes.004");
	for (i = 0; names[i] != NULL; i++) {
		(void)cv_dev(p, sizeof p, names[i]);
		if (stat(p, &st) == 0)
			present++;
	}
	if (present == 0)
		pc_notinuse("neither /dev/urandom nor /dev/random exists; the layout sources do not require either");
	pc_test();
	for (i = 0; names[i] != NULL; i++) {
		(void)cv_dev(p, sizeof p, names[i]);
		if (stat(p, &st) != 0)
			continue;
		pc_check(S_ISCHR(st.st_mode), "%s is not a character special file", p);
		fd = open(p, O_RDONLY | O_NONBLOCK);
		pc_check(fd >= 0, "cannot open %s for reading: errno %d", p, errno);
		r = read(fd, a, sizeof a);
		pc_check(r == (ssize_t)sizeof a, "first read of %s returned %zd, errno %d", p, r, errno);
		r = read(fd, b, sizeof b);
		pc_check(r == (ssize_t)sizeof b, "second read of %s returned %zd, errno %d", p, r, errno);
		(void)close(fd);
		same = 1;
		for (j = 1; j < (int)sizeof a; j++)
			if (a[j] != a[0])
				same = 0;
		pc_check(!same, "%s returned 32 identical bytes", p);
		pc_check(memcmp(a, b, sizeof a) != 0, "%s returned the same 32 bytes twice", p);
	}
	pc_pass();
}
