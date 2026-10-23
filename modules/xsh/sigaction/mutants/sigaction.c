/*
 * sigaction.c - planted faults in sigaction(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "sigaction.<fault>" and apply only
 * while PC_PHASE is "test" (as set by pc.c in this process). Faults that
 * the kernel would otherwise hide (resetting a handler, dropping the
 * siginfo_t) install a trampoline that does the wrong thing and then
 * calls the application's function.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef int (*sigaction_fn)(int, const struct sigaction *, struct sigaction *);

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

static const char *mutant(void)
{
	const char *m = getenv("PC_MUTANT");

	if (strcmp(phase, "test") != 0 || m == NULL || strncmp(m, "sigaction.", 10) != 0)
		return NULL;
	return m + 10;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

static sigaction_fn real_sigaction(void)
{
	static sigaction_fn f;
	if (f == NULL)
		f = (sigaction_fn)dlsym(RTLD_NEXT, "sigaction");
	return f;
}

/* What the application asked for, per signal, when a trampoline stands in. */
static struct sigaction wanted[65];
static volatile sig_atomic_t tramp_on[65];

static void trampoline(int sig)
{
	const char *m = mutant();
	struct sigaction w;

	if (sig <= 0 || sig > 64)
		return;
	w = wanted[sig];
	if (IS("resethand_always")) {
		struct sigaction d;
		memset(&d, 0, sizeof d);
		d.sa_handler = SIG_DFL;
		sigemptyset(&d.sa_mask);
		(void)real_sigaction()(sig, &d, NULL);
		tramp_on[sig] = 0;
	}
	if (w.sa_flags & SA_SIGINFO)
		w.sa_sigaction(sig, NULL, NULL);	/* the siginfo_t is lost */
	else if (w.sa_handler != SIG_DFL && w.sa_handler != SIG_IGN)
		w.sa_handler(sig);
}

int sigaction(int sig, const struct sigaction *act, struct sigaction *oact)
{
	const char *m = mutant();
	struct sigaction a;
	int r;

	if (m == NULL)
		return real_sigaction()(sig, act, oact);
	if (IS("einval_accepted") && act != NULL &&
	    (sig == SIGKILL || sig == SIGSTOP || sig <= 0 || sig > 64))
		return 0;
	if (IS("einval_accepted") && act == NULL && (sig <= 0 || sig > 64)) {
		if (oact != NULL)
			memset(oact, 0, sizeof *oact);
		return 0;
	}
	if (IS("act_ignored") && act != NULL)
		act = NULL;
	if (IS("query_resets") && act == NULL && sig > 0 && sig < 65 && sig != SIGKILL && sig != SIGSTOP) {
		r = real_sigaction()(sig, NULL, oact);
		memset(&a, 0, sizeof a);
		a.sa_handler = SIG_DFL;
		sigemptyset(&a.sa_mask);
		(void)real_sigaction()(sig, &a, NULL);
		return r;
	}
	if (act != NULL) {
		a = *act;
		if (IS("mask_ignored"))
			sigemptyset(&a.sa_mask);
		if (IS("nodefer_always"))
			a.sa_flags |= SA_NODEFER;
		if (IS("nodefer_ignored"))
			a.sa_flags &= ~SA_NODEFER;
		if (IS("restart_ignored"))
			a.sa_flags &= ~SA_RESTART;
		if (IS("restart_always"))
			a.sa_flags |= SA_RESTART;
		if (IS("resethand_ignored"))
			a.sa_flags &= ~SA_RESETHAND;
		if (IS("nocldstop_ignored") && sig == SIGCHLD)
			a.sa_flags &= ~SA_NOCLDSTOP;
		if (IS("nocldstop_always") && sig == SIGCHLD)
			a.sa_flags |= SA_NOCLDSTOP;
		if (IS("nocldstop_kills_all") && sig == SIGCHLD && (a.sa_flags & SA_NOCLDSTOP)) {
			a.sa_handler = SIG_IGN;	/* no SIGCHLD reaches the process at all */
			a.sa_flags &= ~SA_SIGINFO;
		}
		if (IS("onstack_ignored"))
			a.sa_flags &= ~SA_ONSTACK;
		if (IS("onstack_always"))
			a.sa_flags |= SA_ONSTACK;
		if (sig > 0 && sig < 65) {
			int caught = (a.sa_flags & SA_SIGINFO) ||
			    (a.sa_handler != SIG_DFL && a.sa_handler != SIG_IGN);
			int t = caught && ((IS("siginfo_dropped") && (a.sa_flags & SA_SIGINFO)) ||
			    IS("resethand_always"));
			if (t) {
				wanted[sig] = a;
				a.sa_handler = trampoline;
				a.sa_flags &= ~(SA_SIGINFO | SA_RESETHAND);
				tramp_on[sig] = 1;
			}
			else
				tramp_on[sig] = 0;
		}
		act = &a;
	}
	r = real_sigaction()(sig, act, oact);
	if (r == 0 && oact != NULL) {
		if (sig > 0 && sig < 65 && tramp_on[sig] && oact->sa_handler == trampoline && act == NULL)
			*oact = wanted[sig];	/* report what the application set */
		if (IS("oact_wrong_handler"))
			oact->sa_handler = SIG_DFL;
		if (IS("oact_mask_lost"))
			sigemptyset(&oact->sa_mask);
		if (IS("oact_flags_lost"))
			oact->sa_flags = 0;
	}
	return r;
}
