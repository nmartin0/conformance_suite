/*
 * /dev/null can be opened for reading and writing; data written to it is
 * discarded (any amount, with the full count reported) and reading it
 * always gives end-of-file, even after writing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.10.1-directory-structure-and-files.003
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CHUNK (1024 * 1024)

int main(void)
{
	int rw, ro, wo, i;
	char *big;
	char buf[64];

	pc_begin("XBD.10.1-directory-structure-and-files.003");
	big = malloc(CHUNK);
	if (big == NULL)
		pc_unresolved("malloc failed");
	memset(big, 'z', CHUNK);
	pc_test();
	rw = open("/dev/null", O_RDWR);
	pc_check(rw >= 0, "open(\"/dev/null\", O_RDWR) failed: %s", pc_errname(errno));
	ro = open("/dev/null", O_RDONLY);
	pc_check(ro >= 0, "open(\"/dev/null\", O_RDONLY) failed: %s", pc_errname(errno));
	wo = open("/dev/null", O_WRONLY);
	pc_check(wo >= 0, "open(\"/dev/null\", O_WRONLY) failed: %s", pc_errname(errno));
	pc_check(read(rw, buf, sizeof buf) == 0, "a first read of /dev/null did not give end-of-file");
	pc_check(write(rw, "hello", 5) == 5, "write of 5 bytes to /dev/null did not return 5 (errno %s)", pc_errname(errno));
	pc_check(read(rw, buf, sizeof buf) == 0, "a read of /dev/null after a write on the same descriptor did not give end-of-file");
	pc_check(read(ro, buf, sizeof buf) == 0, "a read through another descriptor after the write did not give end-of-file");
	for (i = 0; i < 48; i++)
		pc_check(write(wo, big, CHUNK) == CHUNK, "write of %d bytes to /dev/null (number %d) did not return the whole count", CHUNK, i + 1);
	for (i = 0; i < 4; i++)
		pc_check(read(ro, buf, sizeof buf) == 0, "read number %d of /dev/null did not give end-of-file", i + 1);
	pc_check(read(rw, big, CHUNK) == 0, "a read of %d bytes from /dev/null did not give end-of-file", CHUNK);
	(void)close(rw);
	(void)close(ro);
	(void)close(wo);
	pc_pass();
}
