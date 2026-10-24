/*
 * fcntl.c - conforming variants of fcntl(), for the variant checks (CI
 * only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it. The third argument is read as a pointer for the
 * commands that take one and as an int otherwise.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "pcvariant.h"

typedef int (*fcntl_fn)(int, int, ...);

static fcntl_fn real_fcntl(void)
{
	static fcntl_fn fn;
	if (fn == NULL)
		fn = (fcntl_fn)dlsym(RTLD_NEXT, "fcntl");
	return fn;
}

static int takes_pointer(int cmd)
{
	switch (cmd) {
	case F_GETLK: case F_SETLK: case F_SETLKW:
#ifdef F_OFD_GETLK
	case F_OFD_GETLK: case F_OFD_SETLK: case F_OFD_SETLKW:
#endif
#ifdef F_GETOWN_EX
	case F_GETOWN_EX: case F_SETOWN_EX:
#endif
		return 1;
	}
	return 0;
}

/*
 * "ok_other": the commands whose RETURN VALUE entry is only "a value other
 * than -1" return -2 on success. XSH fcntl() RETURN VALUE says only that for
 * F_SETFD, F_SETFL, the locking commands, F_SETOWN, F_GETOWN_EX and
 * F_SETOWN_EX; it rules out negative values only for F_GETFD and F_GETFL
 * (and -1 for F_GETOWN), so a success value need not be 0 and may be
 * negative.
 */
static int other_than_minus_one(int cmd)
{
	switch (cmd) {
	case F_SETFD: case F_SETFL: case F_GETLK: case F_SETLK: case F_SETLKW:
	case F_SETOWN:
#ifdef F_OFD_GETLK
	case F_OFD_GETLK: case F_OFD_SETLK: case F_OFD_SETLKW:
#endif
#ifdef F_GETOWN_EX
	case F_GETOWN_EX: case F_SETOWN_EX:
#endif
		return 1;
	}
	return 0;
}

/*
 * "cloexec_bit": FD_CLOEXEC is 0x40 (the header half is
 * fcntl.cloexec_bit.h). XBD <fcntl.h> asks only that the descriptor flags
 * be bitwise-distinct constants usable in #if, so any single bit will do.
 * On this system the reference system's own FD_CLOEXEC bit is no flag at
 * all, and F_SETFD drops it.
 */
#define ALT_CLOEXEC 0x40

/*
 * "setlkw_poll": a waiting F_SETLKW or F_OFD_SETLKW is woken late: the
 * waiter retries only every 700 ms. XSH fcntl() F_SETLKW asks that the
 * thread wait until the request can be satisfied and sets no bound on how
 * soon after that it gets the lock. The range is fixed before waiting (as
 * the DESCRIPTION requires), and a caught signal during the wait ends it
 * with EINTR and no lock.
 */
static int poll_lock(int fd, int cmd, struct flock *fl)
{
	fcntl_fn f = real_fcntl();
	struct flock w = *fl;
	int nb = F_SETLK, r;

#ifdef F_OFD_SETLKW
	if (cmd == F_OFD_SETLKW)
		nb = F_OFD_SETLK;
#endif
	if (w.l_type == F_UNLCK)
		return f(fd, cmd, fl);
	if (w.l_whence == SEEK_CUR || w.l_whence == SEEK_END) {
		struct stat st;
		off_t base = -1;
		if (w.l_whence == SEEK_CUR)
			base = lseek(fd, 0, SEEK_CUR);
		else if (fstat(fd, &st) == 0)
			base = st.st_size;
		if (base == -1)
			return f(fd, cmd, fl);
		w.l_start += base;
		w.l_whence = SEEK_SET;
	}
	for (;;) {
		struct timespec ts = { 0, 700000000L };
		r = f(fd, nb, &w);
		if (r != -1 || (errno != EAGAIN && errno != EACCES))
			return r;
		if (nanosleep(&ts, NULL) == -1 && errno == EINTR)
			return -1;	/* errno is EINTR */
	}
}


/*
 * "enolck_small": the system's table of locked regions is tiny: a process
 * may hold one locked region at a time (process-owned, or OFD-owned
 * through its descriptors), and a lock or unlock request that would need
 * more fails with ENOLCK. XSH fcntl() ERRORS requires ENOLCK when a
 * request would exceed a system-imposed limit on locked regions and sets
 * no minimum for that limit, so one region is allowed. The library keeps
 * its own picture of this process's regions (adjacent regions of one type
 * count as one) to know when a request would exceed it.
 */
#define LOCK_LIMIT 1
#define NREG 64
struct reg { dev_t dev; ino_t ino; long owner; off_t s, e; short type; };
static struct reg regs[NREG];
static int nregs;
static pid_t regs_pid;

static off_t off_max(void)
{
	off_t m = 0;
	size_t i;

	for (i = 0; i + 1 < sizeof(off_t) * 8; i++)
		m = m * 2 + 1;
	return m;
}

/* Process-owned locks are not inherited: a child starts with none. */
static void regs_fork_check(void)
{
	int i, j = 0;

	if (regs_pid == getpid())
		return;
	if (regs_pid != 0)
		for (i = 0; i < nregs; i++)
			if (regs[i].owner != 0)
				regs[j++] = regs[i];
	nregs = regs_pid != 0 ? j : 0;
	regs_pid = getpid();
}

/* The absolute byte range of *fl on fd, or -1 if the real call must judge it. */
static int lock_range(int fd, const struct flock *fl, off_t *s, off_t *e)
{
	off_t base = 0, st, max = off_max();
	struct stat sb;

	if (fl->l_whence == SEEK_CUR)
		base = lseek(fd, 0, SEEK_CUR);
	else if (fl->l_whence == SEEK_END)
		base = fstat(fd, &sb) == 0 ? sb.st_size : -1;
	else if (fl->l_whence != SEEK_SET)
		return -1;
	if (base < 0 || (fl->l_start > 0 && base > max - fl->l_start))
		return -1;
	st = base + fl->l_start;
	if (fl->l_len > 0) {
		if (st > max - (fl->l_len - 1))
			return -1;
		*s = st;
		*e = st + fl->l_len - 1;
	} else if (fl->l_len < 0) {
		*s = st + fl->l_len;
		*e = st - 1;
	} else {
		*s = st;
		*e = max;
	}
	return *s < 0 || *s > *e ? -1 : 0;
}

static int same_key(const struct reg *a, const struct reg *b)
{
	return a->dev == b->dev && a->ino == b->ino && a->owner == b->owner;
}

/*
 * The regions after applying *fl (owner 0, or fd + 1 for OFD) into out;
 * returns their number, or -1 when the picture cannot be worked out.
 */
static int regs_after(int fd, int ofd, const struct flock *fl, struct reg *out)
{
	struct reg k;
	struct stat sb;
	off_t s, e;
	int i, n = 0, merged;

	if (fstat(fd, &sb) != 0 || lock_range(fd, fl, &s, &e) != 0)
		return -1;
	k.dev = sb.st_dev;
	k.ino = sb.st_ino;
	k.owner = ofd ? fd + 1 : 0;
	for (i = 0; i < nregs; i++) {
		struct reg r = regs[i];
		if (!same_key(&r, &k) || r.e < s || r.s > e) {
			if (n >= NREG) return -1;
			out[n++] = r;
			continue;
		}
		if (r.s < s) {
			if (n >= NREG) return -1;
			out[n] = r;
			out[n++].e = s - 1;
		}
		if (r.e > e) {
			if (n >= NREG) return -1;
			out[n] = r;
			out[n++].s = e + 1;
		}
	}
	if (fl->l_type != F_UNLCK) {
		if (n >= NREG) return -1;
		k.s = s;
		k.e = e;
		k.type = fl->l_type;
		out[n++] = k;
	}
	do {	/* regions of one type that touch count as one */
		int j;
		merged = 0;
		for (i = 0; i < n && !merged; i++)
			for (j = 0; j < n && !merged; j++) {
				if (i == j || !same_key(&out[i], &out[j]) || out[i].type != out[j].type)
					continue;
				if (out[j].s > out[i].e && out[j].s - out[i].e > 1)
					continue;
				if (out[i].s > out[j].e && out[i].s - out[j].e > 1)
					continue;
				if (out[j].s < out[i].s) out[i].s = out[j].s;
				if (out[j].e > out[i].e) out[i].e = out[j].e;
				out[j] = out[--n];
				merged = 1;
			}
	} while (merged);
	return n;
}

static int limited_lock(int fd, int cmd, struct flock *fl)
{
	fcntl_fn f = real_fcntl();
	static struct reg next[NREG];
	int ofd = 0, n, r;

#ifdef F_OFD_SETLK
	ofd = cmd == F_OFD_SETLK || cmd == F_OFD_SETLKW;
#endif
	regs_fork_check();
	n = regs_after(fd, ofd, fl, next);
	if (n > LOCK_LIMIT) {
		errno = ENOLCK;
		return -1;
	}
	r = f(fd, cmd, fl);
	if (r != -1 && n >= 0) {
		memcpy(regs, next, (size_t)n * sizeof next[0]);
		nregs = n;
	}
	return r;
}

/* Closing a descriptor drops this process's locks on the file, and its OFD picture. */
int close(int fd)
{
	const char *v = variant_of("fcntl.");
	struct stat sb;
	REAL(close);

	if (IS("enolck_small") && fstat(fd, &sb) == 0) {
		int i, j = 0;
		regs_fork_check();
		for (i = 0; i < nregs; i++)
			if (!((regs[i].owner == 0 && regs[i].dev == sb.st_dev &&
			    regs[i].ino == sb.st_ino) || regs[i].owner == fd + 1))
				regs[j++] = regs[i];
		nregs = j;
	}
	return real_close(fd);
}

static int varied(const char *v, int fd, int cmd, long iarg, void *parg)
{
	fcntl_fn f = real_fcntl();
	int r;

	if (IS("cloexec_bit") && cmd == F_SETFD) {
		long a = iarg & ~(long)(ALT_CLOEXEC | FD_CLOEXEC);
		if (iarg & ALT_CLOEXEC)
			a |= FD_CLOEXEC;
		return f(fd, cmd, (int)a);
	}
	if (IS("cloexec_bit") && cmd == F_GETFD) {
		r = f(fd, cmd);
		if (r != -1)
			r = (r & ~FD_CLOEXEC) | ((r & FD_CLOEXEC) ? ALT_CLOEXEC : 0);
		return r;
	}
	if (IS("enolck_small") && (cmd == F_SETLK || cmd == F_SETLKW
#ifdef F_OFD_SETLK
	    || cmd == F_OFD_SETLK || cmd == F_OFD_SETLKW
#endif
	    ))
		return limited_lock(fd, cmd, parg);
	if (IS("setlkw_poll") && (cmd == F_SETLKW
#ifdef F_OFD_SETLKW
	    || cmd == F_OFD_SETLKW
#endif
	    ))
		return poll_lock(fd, cmd, parg);
	if (takes_pointer(cmd))
		r = f(fd, cmd, parg);
	else
		r = f(fd, cmd, (int)iarg);
	/*
	 * "setlk_eacces": a blocked F_SETLK fails with EACCES, not EAGAIN.
	 * XSH fcntl() ERRORS lists [EACCES] or [EAGAIN] for this one
	 * condition; either is allowed. (F_OFD_SETLK has EAGAIN only.)
	 */
	if (IS("setlk_eacces") && cmd == F_SETLK && r == -1 && errno == EAGAIN)
		errno = EACCES;
	/*
	 * "dupfd_max_emfile": F_DUPFD and F_DUPFD_CLOEXEC with a non-negative
	 * arg at or above {OPEN_MAX} fail with EMFILE, not EINVAL. Then no
	 * descriptor at or above arg can be available, so the EMFILE condition
	 * of XSH fcntl() ERRORS holds as well as the EINVAL one, and XSH 2.3
	 * lets the implementation report either.
	 */
	if (IS("dupfd_max_emfile") && (cmd == F_DUPFD || cmd == F_DUPFD_CLOEXEC) &&
	    r == -1 && errno == EINVAL && iarg >= 0)
		errno = EMFILE;
#ifdef F_GETOWN_EX
	/*
	 * "getown_ex_pid0_type": with no SIGURG recipient, F_GETOWN_EX reports
	 * pid 0 with type F_OWNER_PGRP. XSH fcntl() F_GETOWN_EX requires pid 0
	 * then and says nothing about type in that case.
	 */
	if (IS("getown_ex_pid0_type") && cmd == F_GETOWN_EX && r != -1 &&
	    ((struct f_owner_ex *)parg)->pid == 0)
		((struct f_owner_ex *)parg)->type = F_OWNER_PGRP;
#endif
	if (IS("ok_other") && r != -1 && other_than_minus_one(cmd))
		r = -2;
	return r;
}

static int dispatch(int fd, int cmd, va_list ap)
{
	const char *v = variant_of("fcntl.");
	long iarg = 0;
	void *parg = NULL;

	if (takes_pointer(cmd))
		parg = va_arg(ap, void *);
	else
		iarg = va_arg(ap, int);	/* unused by the commands without one */
	if (v == NULL)
		return takes_pointer(cmd) ? real_fcntl()(fd, cmd, parg) :
		    real_fcntl()(fd, cmd, (int)iarg);
	return varied(v, fd, cmd, iarg, parg);
}

int fcntl(int fd, int cmd, ...)
{
	va_list ap;
	int r;

	va_start(ap, cmd);
	r = dispatch(fd, cmd, ap);
	va_end(ap);
	return r;
}

#ifdef __GLIBC__
/* glibc may route calls through fcntl64; vary it the same way. */
int fcntl64(int fd, int cmd, ...)
{
	va_list ap;
	int r;

	va_start(ap, cmd);
	r = dispatch(fd, cmd, ap);
	va_end(ap);
	return r;
}
#endif

/*
 * "open_max_indet": sysconf(_SC_OPEN_MAX) reports no definite limit (-1,
 * errno unchanged). XSH sysconf() returns -1 without changing errno for a
 * variable that has no determinate value, and XBD <limits.h> lets
 * {OPEN_MAX} be omitted when it is indeterminate; the F_DUPFD EINVAL case
 * for arg >= {OPEN_MAX} then has no value to test against.
 */
long sysconf(int name)
{
	const char *v = variant_of("fcntl.");
	REAL(sysconf);

	if (IS("open_max_indet") && name == _SC_OPEN_MAX)
		return -1;
	return real_sysconf(name);
}
