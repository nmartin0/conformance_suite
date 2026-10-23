/*
 * fcntl.c - planted faults in fcntl(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; faults are named "fcntl.<fault>" and apply only
 * while PC_PHASE is "test". The third argument is read as a pointer for
 * the commands that take one and as an int otherwise.
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

typedef int (*fcntl_fn)(int, int, ...);

static fcntl_fn real_fcntl(void)
{
	static fcntl_fn fn;
	if (fn == NULL)
		fn = (fcntl_fn)dlsym(RTLD_NEXT, "fcntl");
	return fn;
}

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");
	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL ||
	    strncmp(m, "fcntl.", 6) != 0)
		return NULL;
	return m + 6;
}

#define IS(name) (m != NULL && strcmp(m, name) == 0)

static void cannot_plant(const char *m)
{
	fprintf(stderr, "pcmutant: cannot plant fcntl.%s here\n", m);
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

/* Every other descriptor open on the same file as fd. */
static int same_file(int fd, int other)
{
	struct stat a, b;
	return other != fd && real_fcntl()(other, F_GETFD) != -1 &&
	    fstat(fd, &a) == 0 && fstat(other, &b) == 0 &&
	    a.st_dev == b.st_dev && a.st_ino == b.st_ino;
}


/* Faults in process-owned record locking (F_GETLK, F_SETLK, F_SETLKW). */
static int lock_fault(const char *m, int fd, int cmd, struct flock *fl)
{
	fcntl_fn f = real_fcntl();
	struct flock in = *fl, w = *fl;
	int r, e;

	if (cmd == F_GETLK) {
		if (IS("getlk_never_conflicts")) {
			r = f(fd, cmd, fl);
			if (r != -1)
				fl->l_type = F_UNLCK;
			return r;
		}
		r = f(fd, cmd, fl);
		if (r == -1)
			return r;
		if (fl->l_type == F_UNLCK) {
			if (IS("getlk_clobbers")) {
				fl->l_start = 0;
				fl->l_len = 0;
				fl->l_pid = 0;
			}
		} else {
			if (IS("getlk_pid_zero"))
				fl->l_pid = 0;
			if (IS("getlk_whence_cur"))
				fl->l_whence = SEEK_CUR;
			if (IS("getlk_echoes_request")) {
				fl->l_start = in.l_start;
				fl->l_len = in.l_len;
			}
		}
		return r;
	}
	/* F_SETLK, F_SETLKW */
	if (IS("setlk_ignored") && w.l_type != F_UNLCK)
		return 0;
	if (IS("setlk_rd_as_wr") && w.l_type == F_RDLCK)
		w.l_type = F_WRLCK;
	if (IS("setlkw_as_setlk"))
		cmd = F_SETLK;
	if (IS("setlk_spurious_eagain") && w.l_type == F_WRLCK) {
		errno = EAGAIN;
		return -1;
	}
	if (IS("setlk_len_plus_one") && w.l_len > 0)
		w.l_len++;
	if (IS("neg_len_unsupported") && w.l_len < 0) {
		errno = EINVAL;
		return -1;
	}
	if (IS("len0_to_eof") && w.l_len == 0 && w.l_type != F_UNLCK) {
		struct stat st;
		if (fstat(fd, &st) == 0 && w.l_whence == SEEK_SET &&
		    st.st_size > w.l_start)
			w.l_len = st.st_size - w.l_start;
		else if (fstat(fd, &st) == 0 && w.l_whence == SEEK_SET)
			w.l_len = 1;
	}
	if (IS("whence_ignored"))
		w.l_whence = SEEK_SET;
	if (IS("unlock_fails") && w.l_type == F_UNLCK) {
		errno = EINVAL;
		return -1;
	}
	if (IS("unlock_whole") && w.l_type == F_UNLCK) {
		w.l_whence = SEEK_SET;
		w.l_start = 0;
		w.l_len = 0;
	}
	if (IS("before_start_ok") && w.l_whence == SEEK_SET && w.l_start < 0) {
		w.l_len += w.l_start;
		w.l_start = 0;
		if (w.l_len <= 0)
			return 0;
	}
	r = f(fd, cmd, &w);
	e = errno;
	if (r == -1) {
		if (IS("setlk_conflict_ok") && (e == EAGAIN || e == EACCES))
			return 0;
		if (IS("setlk_conflict_einval") && (e == EAGAIN || e == EACCES))
			e = EINVAL;
		if (IS("ebadf_mode_ok") && e == EBADF)
			return 0;
		if (IS("eintr_as_eagain") && e == EINTR)
			e = EAGAIN;
		errno = e;
	}
	return r;
}


#ifdef F_OFD_SETLK
/* Faults in OFD-owned record locking. */
static int ofd_fault(const char *m, int fd, int cmd, struct flock *fl)
{
	fcntl_fn f = real_fcntl();
	int r;

	if (IS("ofd_setlk_ignored") && cmd != F_OFD_GETLK && fl->l_type != F_UNLCK)
		return 0;
	if (IS("ofd_as_posix")) {
		/* Owned by the process instead of the open file description. */
		struct flock w = *fl;
		w.l_pid = 0;
		return f(fd, cmd == F_OFD_GETLK ? F_GETLK :
		    cmd == F_OFD_SETLK ? F_SETLK : F_SETLKW, &w);
	}
	if (IS("ofd_setlkw_as_setlk") && cmd == F_OFD_SETLKW)
		cmd = F_OFD_SETLK;
	r = f(fd, cmd, fl);
	if (r == -1 && IS("ofd_conflict_eacces") && errno == EAGAIN)
		errno = EACCES;
	if (r == -1 && IS("eintr_as_eagain") && errno == EINTR)
		errno = EAGAIN;
	if (r != -1 && cmd == F_OFD_GETLK && fl->l_type != F_UNLCK &&
	    IS("ofd_getlk_pid_wrong"))
		fl->l_pid = fl->l_pid == -1 ? 0 : -1;
	if (r != -1 && cmd == F_OFD_GETLK && fl->l_type == F_UNLCK &&
	    IS("getlk_clobbers")) {
		fl->l_start = 0;
		fl->l_len = 0;
	}
	return r;
}
#endif

static int faulty(const char *m, int fd, int cmd, long iarg, void *parg)
{
	fcntl_fn f = real_fcntl();
	int r, i;

	if (cmd == F_DUPFD || cmd == F_DUPFD_CLOEXEC) {
		int c = cmd;
		if (IS("dupfd_cloexec_ignored"))
			c = F_DUPFD;
		if (IS("dupfd_ignores_arg"))
			iarg = 0;
		if (IS("dupfd_neg_ok") && iarg < 0)
			iarg = 0;
		if (IS("dupfd_fresh")) {
			char path[64];
			int fl = f(fd, F_GETFL), n;
			snprintf(path, sizeof path, "/proc/self/fd/%d", fd);
			n = fl == -1 ? -1 : open(path, fl & (O_ACCMODE | O_APPEND));
			if (n >= 0) {
				r = f(n, c, (int)iarg);
				(void)close(n);
				return r;
			}
			cannot_plant(m);
		}
		r = f(fd, c, (int)iarg);
		if (r < 0) {
			if (IS("emfile_as_einval") && errno == EMFILE)
				errno = EINVAL;
			if (IS("ebadf_as_einval") && errno == EBADF)
				errno = EINVAL;
			return r;
		}
		if (IS("dupfd_not_lowest")) {
			int n = f(r, F_DUPFD, r + 1);
			if (n >= 0) {
				(void)close(r);
				r = n;
			}
		}
		if (IS("dupfd_keeps_cloexec") && cmd == F_DUPFD)
			(void)f(r, F_SETFD, f(fd, F_GETFD));
		return r;
	}
	if (cmd == F_SETFD) {
		if (IS("setfd_ignored"))
			return 0;
		r = f(fd, cmd, (int)iarg);
		if (r != -1 && IS("setfd_affects_all"))
			for (i = 0; i < 256; i++)
				if (same_file(fd, i))
					(void)f(i, F_SETFD, (int)iarg);
		return r;
	}
	if (cmd == F_GETFL) {
		r = f(fd, cmd);
		if (r != -1 && IS("getfl_no_accmode"))
			r &= ~O_ACCMODE;
		if (r != -1 && IS("getfl_no_append"))
			r &= ~O_APPEND;
		return r;
	}
	if (cmd == F_SETFL) {
		if (IS("setfl_ignored"))
			return 0;
		if (IS("setfl_creation_flags_act")) {
			if (iarg & O_CLOEXEC)
				(void)f(fd, F_SETFD, FD_CLOEXEC);
			if (iarg & O_TRUNC)
				(void)ftruncate(fd, 0);
		}
		r = f(fd, cmd, (int)iarg);
		if (r != -1 && IS("setfl_by_file"))
			for (i = 0; i < 256; i++)
				if (same_file(fd, i))
					(void)f(i, F_SETFL, (int)iarg);
		return r;
	}
	if (cmd == F_GETOWN) {
		r = f(fd, cmd);
		if (r != -1 && IS("getown_zero"))
			r = 0;
		return r;
	}
	if (cmd == F_SETOWN) {
		if (IS("setown_ignored"))
			return 0;
		r = f(fd, cmd, (int)iarg);
		if (r == -1 && errno == ESRCH && IS("setown_esrch_ok"))
			return 0;
		return r;
	}
#ifdef F_GETOWN_EX
	if (cmd == F_SETOWN_EX && IS("setown_ex_ignored"))
		return 0;
	if (cmd == F_GETOWN_EX) {
		r = f(fd, cmd, parg);
		if (r != -1 && IS("getown_ex_type_swapped")) {
			struct f_owner_ex *o = parg;
			o->type = o->type == F_OWNER_PID ? F_OWNER_PGRP : F_OWNER_PID;
		}
		return r;
	}
#endif
	if (cmd == F_SETLK || cmd == F_SETLKW || cmd == F_GETLK)
		return lock_fault(m, fd, cmd, parg);
#ifdef F_OFD_SETLK
	if (cmd == F_OFD_SETLK || cmd == F_OFD_SETLKW || cmd == F_OFD_GETLK)
		return ofd_fault(m, fd, cmd, parg);
#endif
	if (takes_pointer(cmd))
		r = f(fd, cmd, parg);
	else
		r = f(fd, cmd, (int)iarg);
	if (r == -1) {
		if (IS("ebadf_as_einval") && errno == EBADF)
			errno = EINVAL;
		if (IS("badcmd_ok") && errno == EINVAL)
			return 0;
	}
	return r;
}

static int dispatch(int fd, int cmd, va_list ap)
{
	const char *m = mutant();
	long iarg = 0;
	void *parg = NULL;

	if (takes_pointer(cmd))
		parg = va_arg(ap, void *);
	else
		iarg = va_arg(ap, int);	/* unused by the commands without one */
	if (m == NULL)
		return takes_pointer(cmd) ? real_fcntl()(fd, cmd, parg) :
		    real_fcntl()(fd, cmd, (int)iarg);
	return faulty(m, fd, cmd, iarg, parg);
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
/* glibc may route calls through fcntl64; fault it the same way. */
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
