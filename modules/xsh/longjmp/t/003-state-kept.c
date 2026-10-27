/*
 * State as of the longjmp() call survives the jump.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.longjmp.003
 */
#include "pc.h"
#include <fcntl.h>
#include <setjmp.h>
#include <string.h>
#include <unistd.h>

static jmp_buf env;
static int global_counter;
static char heap_like[16];

int main(void)
{
	volatile int local = 1;
	volatile double dv = 1.5;
	int fd;
	off_t at_jump;

	pc_begin("XSH.longjmp.003");
	pc_make_file("state-file", "");
	fd = open("state-file", O_WRONLY);
	pc_check(fd >= 0, "open failed");
	pc_test();
	if (setjmp(env) == 0) {
		local = 20;
		dv = 2.75;
		global_counter = 99;
		strcpy(heap_like, "after-setjmp");
		pc_check(write(fd, "0123456789", 10) == 10, "write failed");
		longjmp(env, 1);
	}
	pc_check(local == 20, "volatile local is %d, expected 20", local);
	pc_check(dv == 2.75, "volatile double is %g, expected 2.75", dv);
	pc_check(global_counter == 99, "global is %d, expected 99", global_counter);
	pc_check(strcmp(heap_like, "after-setjmp") == 0, "static array is \"%s\"", heap_like);
	at_jump = lseek(fd, 0, SEEK_CUR);
	pc_check(at_jump == 10, "file offset is %ld, expected 10", (long)at_jump);
	(void)close(fd);
	pc_pass();
}
