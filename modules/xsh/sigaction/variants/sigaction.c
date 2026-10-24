/*
 * sigaction.c - conforming variants of signal actions and delivery, for the
 * variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it. The XSH 2.4 section tests use these too: their
 * directory names hold dots, which the variant prefix cannot.
 *
 * Several variants put a trampoline between the kernel and a handler the
 * test installs with sigaction(); sigaction() then reports the test's own
 * handler, so the system looks the same from the outside.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "pcvariant.h"

#define NS 129

static void (*user1[NS])(int);
static void (*user3[NS])(int, siginfo_t *, void *);
static int resethand_set[NS];
static void took(const char *v, int sig, const siginfo_t *si);

static void pause_ms(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

/* Done on every delivery through a trampoline, before the test's handler. */
static void before(int sig, siginfo_t *si)
{
	const char *v = variant_of("sigaction.");
	int e = errno;

	/*
	 * "late_delivery": the handler's work is seen 300 ms after the signal
	 * was generated. XSH 2.4.1 sets no time for delivering a pending
	 * signal; only kill(), raise(), pthread_sigmask() and the like must
	 * deliver one before they return, and they still do, as the delay is
	 * inside the delivery.
	 */
	if (IS("late_delivery"))
		pause_ms(300);
	/*
	 * "si_addr_inaccurate": a fault's si_addr is not the faulting address.
	 * XBD <signal.h> lists si_addr for SIGILL, SIGFPE, SIGSEGV and SIGBUS
	 * but says that on some implementations it may be inaccurate.
	 */
	if (IS("si_addr_inaccurate") && si != NULL &&
	    (sig == SIGSEGV || sig == SIGBUS || sig == SIGILL || sig == SIGFPE))
		si->si_addr = (void *)(uintptr_t)16;
	took(v, sig, si);	/* "sigqueue_max_32": a delivery frees a slot */
	errno = e;
}

static void tramp1(int sig)
{
	void (*fn)(int) = sig > 0 && sig < NS ? user1[sig] : NULL;

	before(sig, NULL);
	if (fn != NULL && fn != SIG_DFL && fn != SIG_IGN)
		fn(sig);
}

static void tramp3(int sig, siginfo_t *si, void *uc)
{
	void (*fn)(int, siginfo_t *, void *) = sig > 0 && sig < NS ? user3[sig] : NULL;

	before(sig, si);
	if (fn != NULL)
		fn(sig, si, uc);
}

/* Does the active variant route this action through a trampoline? */
static int wants_tramp(const char *v, int sig, const struct sigaction *act)
{
	if (act == NULL || sig <= 0 || sig >= NS)
		return 0;
	if (act->sa_flags & SA_SIGINFO) {
		if (IS("late_delivery"))
			return 1;
		if (IS("si_addr_inaccurate"))
			return sig == SIGSEGV || sig == SIGBUS || sig == SIGILL || sig == SIGFPE;
		return IS("sigqueue_max_32");
	}
	return IS("late_delivery") && act->sa_handler != SIG_DFL && act->sa_handler != SIG_IGN;
}

/* Put the test's handler (and flags) back in a reported (earlier) action. */
static void unmap(const char *v, struct sigaction *o, void (*u1)(int),
    void (*u3)(int, siginfo_t *, void *), int rh)
{
	if (o == NULL)
		return;
	if ((o->sa_flags & SA_SIGINFO) && o->sa_sigaction == tramp3)
		o->sa_sigaction = u3;
	else if (!(o->sa_flags & SA_SIGINFO) && o->sa_handler == tramp1)
		o->sa_handler = u1;
	if (IS("resethand_nodefer") && (o->sa_flags & SA_RESETHAND) && rh == 1)
		o->sa_flags &= ~SA_NODEFER;
}

int sigaction(int sig, const struct sigaction *act, struct sigaction *oact)
{
	const char *v = variant_of("sigaction.");
	struct sigaction a;
	void (*u1)(int) = NULL;
	void (*u3)(int, siginfo_t *, void *) = NULL;
	int r, rh = 0;
	REAL(sigaction);

	if (v == NULL || sig <= 0 || sig >= NS)
		return real_sigaction(sig, act, oact);
	u1 = user1[sig];
	u3 = user3[sig];
	rh = resethand_set[sig];
	/*
	 * "dfl_uncatchable_ok": setting SIGKILL or SIGSTOP to SIG_DFL is
	 * ignored and succeeds. XSH sigaction() leaves it unspecified whether
	 * that is ignored or fails with EINVAL (Linux fails).
	 */
	if (IS("dfl_uncatchable_ok") && act != NULL && (sig == SIGKILL || sig == SIGSTOP) &&
	    !(act->sa_flags & SA_SIGINFO) && act->sa_handler == SIG_DFL)
		return real_sigaction(sig, NULL, oact);
	/*
	 * "siginfo_rt_only": SA_SIGINFO is refused with EINVAL for a signal
	 * outside SIGRTMIN..SIGRTMAX. XSH sigaction() ERRORS allows this on a
	 * system without the XSI option; tests that need SA_SIGINFO on such
	 * a signal cannot judge that system and must say so (UNRESOLVED),
	 * so the expected result is UNDECIDED. To be consistent, the system
	 * does not claim XSI: sysconf(_SC_XOPEN_UNIX) is -1 (below).
	 */
	if (IS("siginfo_rt_only") && act != NULL && (act->sa_flags & SA_SIGINFO) &&
	    (sig < SIGRTMIN || sig > SIGRTMAX)) {
		errno = EINVAL;
		return -1;
	}
	if (act != NULL) {
		a = *act;
		/*
		 * "resethand_nodefer": with SA_RESETHAND the signal is not
		 * blocked on entry to its handler, as if SA_NODEFER were set:
		 * XSH sigaction() allows exactly that. The flag is not
		 * reported back unless the test set it.
		 */
		if (IS("resethand_nodefer") && (a.sa_flags & SA_RESETHAND)) {
			resethand_set[sig] = (a.sa_flags & SA_NODEFER) ? 2 : 1;
			a.sa_flags |= SA_NODEFER;
		} else
			resethand_set[sig] = 0;
		if (wants_tramp(v, sig, &a)) {
			if (a.sa_flags & SA_SIGINFO) {
				user3[sig] = a.sa_sigaction;
				a.sa_sigaction = tramp3;
			} else {
				user1[sig] = a.sa_handler;
				a.sa_handler = tramp1;
			}
		}
		act = &a;
	}
	r = real_sigaction(sig, act, oact);
	if (r == 0)
		unmap(v, oact, u1, u3, rh);
	else if (act != NULL) {	/* nothing was installed */
		user1[sig] = u1;
		user3[sig] = u3;
		resethand_set[sig] = rh;
	}
	return r;
}

/* ---------- queuing ---------- */

/*
 * "sigqueue_max_32": {SIGQUEUE_MAX} is 32, the least XBD <limits.h> allows
 * (_POSIX_SIGQUEUE_MAX), and sigqueue() fails with EAGAIN once this
 * process has 32 queued signals still pending (XSH sigqueue() ERRORS).
 * The count covers signals the process queues to itself while SA_SIGINFO
 * is set for them. It drops by one when a handler installed with
 * sigaction() is entered for such a signal (through the trampoline) or a
 * sigwait function accepts one, and to zero for a signal number that is no
 * longer pending (discarded by SIG_IGN, or all taken). Limits: a delivery
 * to a handler set with signal(), and signals queued to other processes,
 * are not tracked, so the variant is declared only by tests that queue to
 * themselves and take the signals in one of those ways.
 */
static int queued[NS];
static pid_t owner;

static void sync_owner(void)
{
	if (owner != getpid()) {	/* a fork child has queued nothing */
		owner = getpid();
		memset(queued, 0, sizeof queued);
	}
}

static int has_siginfo(int sig)
{
	struct sigaction sa;
	REAL(sigaction);

	return real_sigaction(sig, NULL, &sa) == 0 && (sa.sa_flags & SA_SIGINFO) != 0;
}

/* One queued occurrence of sig was delivered or accepted (si NULL: unknown which). */
static void took(const char *v, int sig, const siginfo_t *si)
{
	if (!IS("sigqueue_max_32") || sig <= 0 || sig >= NS)
		return;
	sync_owner();
	if (si != NULL && (si->si_code != SI_QUEUE || si->si_pid != getpid()))
		return;
	if (queued[sig] > 0)
		queued[sig]--;
}

static int pending_queued(void)
{
	sigset_t p;
	int s, n = 0;

	sync_owner();
	if (sigpending(&p) != 0)
		return 0;
	for (s = 1; s < NS; s++) {
		if (sigismember(&p, s) != 1)
			queued[s] = 0;
		n += queued[s];
	}
	return n;
}

long sysconf(int name)
{
	const char *v = variant_of("sigaction.");
	REAL(sysconf);

	if (IS("sigqueue_max_32") && name == _SC_SIGQUEUE_MAX)
		return 32;
	/* "siginfo_rt_only" (see sigaction() above): no XSI option */
	if (IS("siginfo_rt_only") && name == _SC_XOPEN_UNIX)
		return -1;
	return real_sysconf(name);
}

int sigtimedwait(const sigset_t *set, siginfo_t *info, const struct timespec *to)
{
	const char *v = variant_of("sigaction.");
	siginfo_t local;
	int r;
	REAL(sigtimedwait);

	if (info == NULL)
		info = &local;
	r = real_sigtimedwait(set, info, to);
	if (r > 0)
		took(v, r, info);
	return r;
}

int sigwaitinfo(const sigset_t *set, siginfo_t *info)
{
	const char *v = variant_of("sigaction.");
	siginfo_t local;
	int r;
	REAL(sigwaitinfo);

	if (info == NULL)
		info = &local;
	r = real_sigwaitinfo(set, info);
	if (r > 0)
		took(v, r, info);
	return r;
}

/* sigwait() reports no siginfo_t: it counts as taking a queued one if any. */
int sigwait(const sigset_t *set, int *sig)
{
	const char *v = variant_of("sigaction.");
	int r;
	REAL(sigwait);

	r = real_sigwait(set, sig);
	if (r == 0)
		took(v, *sig, NULL);
	return r;
}

static int self_blocked(int sig)
{
	sigset_t m;

	return pthread_sigmask(SIG_BLOCK, NULL, &m) == 0 && sigismember(&m, sig) == 1;
}

static int is_pending(int sig)
{
	sigset_t p;

	return sigpending(&p) == 0 && sigismember(&p, sig) == 1;
}

int sigqueue(pid_t pid, int sig, union sigval val)
{
	const char *v = variant_of("sigaction.");
	int r;
	REAL(sigqueue);

	if (IS("sigqueue_max_32") && pid == getpid() && sig > 0 && sig < NS && has_siginfo(sig)) {
		if (pending_queued() >= 32) {
			errno = EAGAIN;
			return -1;
		}
		r = real_sigqueue(pid, sig, val);
		if (r == 0 && is_pending(sig))
			queued[sig]++;
		return r;
	}
	return real_sigqueue(pid, sig, val);
}

int kill(pid_t pid, int sig)
{
	const char *v = variant_of("sigaction.");
	REAL(kill);
	REAL(sigqueue);

	if (v == NULL || pid != getpid() || sig < SIGRTMIN || sig > SIGRTMAX)
		return real_kill(pid, sig);
	/*
	 * "kill_not_queued": kill() of a realtime signal that is already
	 * pending adds no occurrence. XSH 2.4.2 requires queuing only for
	 * sigqueue() and other functions that carry a value; a kill() for
	 * which the implementation does not queue leaves the queued ones as
	 * they are.
	 */
	if (IS("kill_not_queued") && is_pending(sig))
		return real_kill(pid, 0);
	/*
	 * "kill_first": the kill() occurrence is accepted before the ones
	 * sigqueue() queued earlier. XSH 2.4.2 orders only the occurrences
	 * that are queued with values (FIFO); where an unqueued kill() goes
	 * is not specified (standard question Q-17). Planted by taking the
	 * queued ones off (the signal is blocked here) and queuing them again
	 * after the kill().
	 */
	if (IS("kill_first") && self_blocked(sig) && is_pending(sig)) {
		union sigval vals[64];
		int codes[64], n = 0, i, r;
		sigset_t s;
		siginfo_t si;
		struct timespec zero = { 0, 0 };

		sigemptyset(&s);
		sigaddset(&s, sig);
		while (n < 64 && sigtimedwait(&s, &si, &zero) == sig) {
			vals[n] = si.si_value;
			codes[n++] = si.si_code;
		}
		r = real_kill(pid, sig);
		for (i = 0; i < n; i++)
			if (codes[i] == SI_QUEUE)
				(void)real_sigqueue(pid, sig, vals[i]);
			else
				(void)real_kill(pid, sig);
		return r;
	}
	return real_kill(pid, sig);
}
