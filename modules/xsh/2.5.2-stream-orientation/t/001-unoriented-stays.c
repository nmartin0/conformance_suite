/*
 * Operations that are not input or output leave a new stream without orientation.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.5.2-stream-orientation.001
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <wchar.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

int main(void)
{
	FILE *f;
	fpos_t pos;
	int o;

	pc_begin("XSH.2.5.2-stream-orientation.001");
	pc_make_file("f", "abcdef");
	f = fopen("f", "r+");
	NEED(f != NULL, "fopen failed: %s", pc_errname(errno));
	pc_test();
	/* setvbuf comes before any other operation on the stream, fwide() included */
	pc_check(setvbuf(f, NULL, _IOFBF, 512) == 0, "setvbuf failed");
	o = fwide(f, 0);
	pc_check(o == 0, "a new stream is not without orientation (fwide() gave %d)", o);
	pc_check(feof(f) == 0 && ferror(f) == 0, "feof or ferror on a new stream is true");
	clearerr(f);
	pc_check(fileno(f) >= 0, "fileno failed");
	flockfile(f);
	funlockfile(f);
	pc_check(fflush(f) == 0, "fflush failed");
	pc_check(ftell(f) == 0, "ftell gave a position other than 0");
	pc_check(fseek(f, 2, SEEK_SET) == 0, "fseek failed");
	pc_check(ftell(f) == 2, "ftell after fseek(2) did not give 2");
	pc_check(fgetpos(f, &pos) == 0, "fgetpos failed");
	pc_check(fseek(f, 0, SEEK_END) == 0 && ftell(f) == 6, "fseek to the end failed");
	pc_check(fsetpos(f, &pos) == 0 && ftell(f) == 2, "fsetpos did not restore position 2");
	rewind(f);
	pc_check(ftell(f) == 0, "rewind did not go back to 0");
	o = fwide(f, 0);
	pc_check(o == 0, "after those operations fwide(f, 0) returned %d, not 0", o);
	pc_pass();
}
