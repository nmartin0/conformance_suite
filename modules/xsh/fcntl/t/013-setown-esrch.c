/*
 * F_SETOWN with a process that is gone gives ESRCH.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fcntl.013
 * MUTANT    fcntl.setown_esrch_ok
 * VARIANT   fcntl.ok_other
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>

int main(void)
{
	int sv[2];
	pid_t pid;

	pc_begin("XSH.fcntl.013");
	pc_check(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed");
	pid = fork();
	pc_check(pid != -1, "fork failed");
	if (pid == 0)
		_exit(0);
	pc_check(waitpid(pid, NULL, 0) == pid, "waitpid failed");
	if (kill(pid, 0) == 0 || errno != ESRCH)
		pc_unresolved("process ID %ld was reused", (long)pid);
	pc_test();
	if (fcntl(sv[0], F_SETOWN, (int)pid) != -1 && kill(pid, 0) == 0)
		pc_unresolved("process ID %ld was reused during the test", (long)pid);
	pc_expect_err(fcntl(sv[0], F_SETOWN, (int)pid), ESRCH,
	    "fcntl(F_SETOWN, <process that has exited>)");
	pc_pass();
}
