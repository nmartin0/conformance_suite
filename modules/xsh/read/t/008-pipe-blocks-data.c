/*
 * A blocking read of an empty pipe waits for data.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.008
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
	char c = 0;
	ssize_t r;

	pc_begin("XSH.read.008");
	pc_check(pipe(p) == 0, "pipe failed");
	pid = fork();
	pc_check(pid != -1, "fork failed");
	if (pid == 0) {
		sleep(1);
		_exit(write(p[1], "z", 1) == 1 ? 0 : 1);
	}
	alarm(20);
	pc_test();
	r = read(p[0], &c, 1);
	pc_check(r == 1 && c == 'z', "read() returned %ld (%s) instead of waiting for the data",
	    (long)r, r < 0 ? pc_errname(errno) : "-");
	(void)waitpid(pid, NULL, 0);
	pc_pass();
}
