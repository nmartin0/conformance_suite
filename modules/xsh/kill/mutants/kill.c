/*
 * kill.c - planted faults in kill(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "kill.<fault>" and apply only while
 * PC_PHASE is "test" (as set by pc.c in this process or a parent).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>

static char phase[16];

/* pc.c sets PC_PHASE with setenv(); remember it in this process. */
int setenv(const char *name, const char *value, int overwrite)
{
	static int (*real)(const char *, const char *, int);

	if (real == NULL)
		real = (int (*)(const char *, const char *, int))dlsym(RTLD_NEXT, "setenv");
	if (strcmp(name, "PC_PHASE") == 0 && strlen(value) < sizeof phase)
		strcpy(phase, value);
	return real(name, value, overwrite);
}

static const char *mutant_of(const char *prefix)
{
	const char *m = getenv("PC_MUTANT");
	size_t n = strlen(prefix);

	if (strcmp(phase, "test") != 0 || m == NULL || strncmp(m, prefix, n) != 0)
		return NULL;
	return m + n;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

static int valid(int sig)
{
	return sig >= 0 && sig < 65;
}

int kill(pid_t pid, int sig)
{
	static int (*real)(pid_t, int);
	const char *m = mutant_of("kill.");

	if (real == NULL)
		real = (int (*)(pid_t, int))dlsym(RTLD_NEXT, "kill");
	if (m == NULL)
		return real(pid, sig);
	if (IS("einval_accepted") && !valid(sig))
		return 0;
	if (IS("self_to_calling_thread") && pid == getpid() && sig > 0)
		return pthread_kill(pthread_self(), sig) == 0 ? 0 : -1;	/* to the thread, not the process */
	if (IS("drops") && sig != 0 && sig != SIGKILL && pid > 0 && pid != getpid())
		return 0;
	if (IS("null_sends") && sig == 0)
		sig = SIGUSR1;
	if (IS("zero_as_self") && pid == 0)
		pid = getpid();
	if (IS("group_as_pid") && pid < -1)
		pid = -pid;
	if (IS("group_ignored") && pid < -1)
		return 0;
	if (IS("all_as_self") && pid == -1)
		pid = getpid();
	if (IS("sigcont_checked") && sig == SIGCONT && geteuid() != 0 && pid != getpid()) {
		errno = EPERM;
		return -1;
	}
	if ((IS("zero_misses_self") && pid == 0) || (IS("deferred") && pid == getpid() && sig > 0)) {
		/* the sender's own copy is lost (zero_misses_self) or held back (deferred) */
		sigset_t one, old;
		struct timespec zero = { 0, 0 };
		int r;
		sigemptyset(&one);
		sigaddset(&one, sig);
		(void)sigprocmask(SIG_BLOCK, &one, &old);
		r = real(pid, sig);
		if (IS("zero_misses_self")) {
			(void)sigtimedwait(&one, NULL, &zero);
			(void)sigprocmask(SIG_SETMASK, &old, NULL);
		}
		return r;
	}
	if (IS("zombie_esrch") && pid > 0) {
		siginfo_t si;
		memset(&si, 0, sizeof si);
		if (waitid(P_PID, (id_t)pid, &si, WEXITED | WNOHANG | WNOWAIT) == 0 && si.si_pid == pid) {
			errno = ESRCH;	/* treats a zombie as gone */
			return -1;
		}
	}
	if (IS("group_plus_self") && pid < -1) {
		int r = real(pid, sig);
		(void)real(getpid(), sig);	/* the sender gets it too */
		return r;
	}
	{
		int r = real(pid, sig);
		if (r == -1 && IS("eperm_hidden") && errno == EPERM)
			return 0;
		if (r == -1 && IS("esrch_as_eperm") && errno == ESRCH)
			errno = EPERM;
		if (r == -1 && IS("eperm_as_esrch") && errno == EPERM)
			errno = ESRCH;
		return r;
	}
}
