/*
 * Closing any descriptor for a file releases the process's fcntl locks on it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.close.005
 * MUTANT    close.noop
 */
#include "pc.h"
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/* 1 if another process sees a lock on all of f, 0 if not. */
static int other_sees_lock(void)
{
	pid_t pid;
	int st;

	pid = fork();
	if (pid == -1)
		pc_unresolved("lock probe: fork failed");
	if (pid == 0) {
		struct flock fl;
		int fd = open("f", O_RDWR);
		if (fd < 0)
			_exit(2);
		memset(&fl, 0, sizeof fl);
		fl.l_type = F_WRLCK;
		fl.l_whence = SEEK_SET;
		if (fcntl(fd, F_GETLK, &fl) != 0)
			_exit(2);
		_exit(fl.l_type == F_UNLCK ? 0 : 1);
	}
	if (waitpid(pid, &st, 0) != pid)
		pc_unresolved("lock probe: waitpid failed");
	if (!WIFEXITED(st) || WEXITSTATUS(st) >= 2)
		pc_unresolved("lock probe in the child failed");
	return WEXITSTATUS(st) == 1;
}

int main(void)
{
	struct flock fl;
	int a, b;

	pc_begin("XSH.close.005");
	pc_make_file("f", "data");
	a = open("f", O_RDWR);
	pc_check(a >= 0, "cannot open f");
	memset(&fl, 0, sizeof fl);
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	pc_check(fcntl(a, F_SETLK, &fl) == 0, "F_SETLK failed");
	b = open("f", O_RDONLY);
	pc_check(b >= 0, "cannot open f again");
	pc_check(other_sees_lock(), "another process does not see the lock");
	pc_test();
	pc_expect_ok(close(b), "close(second descriptor)");
	pc_check(!other_sees_lock(),
	    "lock set through the first descriptor survived closing the second");
	pc_pass();
}
