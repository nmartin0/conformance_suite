/*
 * The child does not hold the parent's process-shared mutexes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fork.014
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>

/*
 * Waits for the child; returns its exit status (0 = all its checks held).
 * A child that did not exit normally is UNRESOLVED.
 */
static int reap(pid_t pid)
{
	int st;

	if (waitpid(pid, &st, 0) != pid)
		pc_unresolved("waitpid failed");
	if (!WIFEXITED(st))
		pc_unresolved("the child was killed by signal %d", WIFSIGNALED(st) ? WTERMSIG(st) : 0);
	return WEXITSTATUS(st);
}

int main(void)
{
	pthread_mutexattr_t a;
	pthread_mutex_t *mx;
	pid_t r;
	int code;

	pc_begin("XSH.fork.014");
	{
		/* File-backed, so as not to depend on MAP_ANONYMOUS (Issue 8). */
		int fd;
		pc_make_file("m", "");
		fd = open("m", O_RDWR);
		pc_check(fd >= 0 && ftruncate(fd, (off_t)sizeof *mx) == 0, "cannot size the file");
		mx = mmap(NULL, sizeof *mx, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		pc_check(mx != MAP_FAILED, "mmap failed");
	}
	pc_check(pthread_mutexattr_init(&a) == 0 &&
	    pthread_mutexattr_setpshared(&a, PTHREAD_PROCESS_SHARED) == 0 &&
	    pthread_mutexattr_settype(&a, PTHREAD_MUTEX_ERRORCHECK) == 0 &&
	    pthread_mutex_init(mx, &a) == 0, "cannot make a process-shared mutex");
	pc_check(pthread_mutex_lock(mx) == 0, "lock failed");
	pc_test();
	r = fork();
	if (r == -1)
		pc_unresolved("fork failed");
	if (r == 0)
		_exit(pthread_mutex_unlock(mx) == EPERM ? 0 : 1);
	code = reap(r);
	pc_check(code == 0, "the child could unlock the parent's process-shared mutex");
	pc_pass();
}
