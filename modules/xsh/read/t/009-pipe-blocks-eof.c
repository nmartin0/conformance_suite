/*
 * A blocking read of an empty pipe ends with 0 when the writers go.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.009
 * MUTANT    read.pipe_block_eagain
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	int p[2];
	pid_t pid;
	char c;
	ssize_t r;

	pc_begin("XSH.read.009");
	pc_check(pipe(p) == 0, "pipe failed");
	pid = fork();
	pc_check(pid != -1, "fork failed");
	if (pid == 0) {
		sleep(1);
		_exit(0);	/* the last writer goes */
	}
	pc_check(close(p[1]) == 0, "close failed");
	alarm(20);
	pc_test();
	r = read(p[0], &c, 1);
	pc_check(r == 0, "read() returned %ld (%s), expected 0 after the writer left",
	    (long)r, r < 0 ? pc_errname(errno) : "-");
	(void)waitpid(pid, NULL, 0);
	pc_pass();
}
