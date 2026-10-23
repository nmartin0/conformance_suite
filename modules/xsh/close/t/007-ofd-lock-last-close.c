/*
 * An OFD lock lasts until the last descriptor for its description closes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.close.007
 * MUTANT    close.noop
 * MUTANT    close.ofd_unlock_each
 */
#include "pc.h"
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#ifdef F_OFD_SETLK

/* 1 if another process sees a lock on all of f, 0 if not. */
static int other_sees_lock(void)
{
	pid_t pid;
	int st;

	pid = fork();
	pc_check(pid != -1, "fork failed");
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
	pc_check(waitpid(pid, &st, 0) == pid, "waitpid failed");
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) < 2,
	    "lock probe in the child failed");
	return WEXITSTATUS(st) == 1;
}
#endif

int main(void)
{
#ifdef F_OFD_SETLK
	struct flock fl;
	int a, b;
#endif

	pc_begin("XSH.close.007");
	pc_make_file("f", "data");
#ifndef F_OFD_SETLK
	pc_unresolved("<fcntl.h> does not define F_OFD_SETLK");
#else
	a = open("f", O_RDWR);
	pc_check(a >= 0, "cannot open f");
	memset(&fl, 0, sizeof fl);
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_pid = 0;
	pc_check(fcntl(a, F_OFD_SETLK, &fl) == 0, "F_OFD_SETLK failed");
	b = dup(a);
	pc_check(b >= 0, "dup failed");
	pc_check(other_sees_lock(), "another process does not see the OFD lock");
	pc_test();
	pc_expect_ok(close(a), "close(first descriptor)");
	pc_check(other_sees_lock(),
	    "OFD lock released while a descriptor for the description is still open");
	pc_expect_ok(close(b), "close(last descriptor)");
	pc_check(!other_sees_lock(), "OFD lock not released at the last close");
	pc_pass();
#endif
}
