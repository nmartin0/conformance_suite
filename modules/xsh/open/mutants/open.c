/*
 * open.c - planted faults in open(), for mutation checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Built as a preload library. It interposes open() and, only while
 * PC_PHASE is "test", applies the fault named by PC_MUTANT. Each test
 * lists the mutants it must catch; ci/mutate.sh checks that every one
 * turns the test's PASS into FAIL.
 *
 * This is harness code, not a test: it uses dlsym(RTLD_NEXT).
 */
#define _GNU_SOURCE 1
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

typedef int (*open_fn)(const char *, int, ...);
typedef int (*openat_fn)(int, const char *, int, ...);

static openat_fn real_openat(void)
{
	static openat_fn fn;
	if (fn == NULL)
		fn = (openat_fn)dlsym(RTLD_NEXT, "openat");
	return fn;
}

static open_fn real_open(void)
{
	static open_fn fn;
	if (fn == NULL)
		fn = (open_fn)dlsym(RTLD_NEXT, "open");
	return fn;
}

static const char *mutant(void)
{
	const char *ph = getenv("PC_PHASE");
	const char *m = getenv("PC_MUTANT");
	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL)
		return NULL;
	if (strncmp(m, "open.", 5) != 0)
		return NULL;
	return m + 5;
}

int open(const char *path, int flags, ...)
{
	mode_t mode = 0;
	const char *m;
	struct stat st, pst, old;
	int fd, e, existed = 0, have_parent = 0, have_old = 0;
	const char *parent = ".";

	if (flags & O_CREAT) {
		va_list ap;
		va_start(ap, flags);
		mode = (mode_t)va_arg(ap, int);
		va_end(ap);
	}
	m = mutant();
	if (m == NULL)
		return real_open()(path, flags, mode);

#define IS(name) (strcmp(m, name) == 0)
	/* Faults applied to the arguments. */
	if (IS("creat_ignored"))
		flags &= ~O_CREAT;
	if (IS("excl_ignored"))
		flags &= ~O_EXCL;
	if (IS("trunc_ignored"))
		flags &= ~O_TRUNC;
	if (IS("append_ignored"))
		flags &= ~O_APPEND;
	if (IS("cloexec_ignored"))
		flags &= ~O_CLOEXEC;
	if (IS("cloexec_forced"))
		flags |= O_CLOEXEC;
	if (IS("directory_ignored"))
		flags &= ~O_DIRECTORY;
	if (IS("nofollow_ignored"))
		flags &= ~O_NOFOLLOW;
#ifdef O_CLOFORK
	if (IS("clofork_ignored"))
		flags &= ~O_CLOFORK;
	if (IS("clofork_forced"))
		flags |= O_CLOFORK;
#endif
	if (IS("rdonly_writable") && (flags & O_ACCMODE) == O_RDONLY)
		flags = (flags & ~O_ACCMODE) | O_RDWR;
	if (IS("wronly_readable") && (flags & O_ACCMODE) == O_WRONLY)
		flags = (flags & ~O_ACCMODE) | O_RDWR;
	if (IS("rdwr_as_rdonly") && (flags & O_ACCMODE) == O_RDWR)
		flags = (flags & ~O_ACCMODE) | O_RDONLY;
	if (IS("rdwr_as_wronly") && (flags & O_ACCMODE) == O_RDWR)
		flags = (flags & ~O_ACCMODE) | O_WRONLY;
	if (IS("nonblock_ignored"))
		flags &= ~O_NONBLOCK;
	if (IS("fifo_never_blocks") && stat(path, &st) == 0 &&
	    S_ISFIFO(st.st_mode))
		flags |= O_NONBLOCK;
	if (IS("fifo_block_enxio") && !(flags & O_NONBLOCK) &&
	    stat(path, &st) == 0 && S_ISFIFO(st.st_mode)) {
		errno = ENXIO;
		return -1;
	}
	if (IS("nonblock_regular_eagain") && (flags & O_NONBLOCK) &&
	    stat(path, &st) == 0 && S_ISREG(st.st_mode)) {
		errno = EAGAIN;
		return -1;
	}
	if (IS("noctty_enotty") && (flags & O_NOCTTY) &&
	    stat(path, &st) == 0 && !S_ISCHR(st.st_mode)) {
		errno = ENOTTY;
		return -1;
	}
#ifdef O_TTY_INIT
	if (IS("tty_init_enotty") && O_TTY_INIT != 0 && (flags & O_TTY_INIT) &&
	    stat(path, &st) == 0 && !S_ISCHR(st.st_mode)) {
		errno = ENOTTY;
		return -1;
	}
#endif
	if (IS("creat_truncates_existing") && (flags & O_CREAT) &&
	    stat(path, &st) == 0)
		flags |= O_TRUNC;
	if (IS("creat_resets_mode") && (flags & O_CREAT) &&
	    stat(path, &st) == 0)
		(void)chmod(path, mode & 0777);
	if (IS("creat_on_dir_ok") && (flags & O_CREAT) &&
	    !(flags & O_DIRECTORY) && stat(path, &st) == 0 &&
	    S_ISDIR(st.st_mode))
		flags &= ~O_CREAT;
	if (IS("trailing_slash_ignored")) {
		static char buf[4096];
		size_t n = strlen(path);
		if (n > 1 && n < sizeof buf && path[n - 1] == '/') {
			memcpy(buf, path, n + 1);
			while (n > 1 && buf[n - 1] == '/')
				buf[--n] = '\0';
			path = buf;
		}
	}
	if (IS("creat_makes_prefix") && (flags & O_CREAT)) {
		static char buf[4096];
		const char *s = strrchr(path, '/');
		size_t n = s ? (size_t)(s - path) : 0;
		if (n > 0 && n < sizeof buf) {
			memcpy(buf, path, n);
			buf[n] = '\0';
			(void)mkdir(buf, 0755);
		}
	}
	if (IS("always_creat")) {
		flags |= O_CREAT;
		mode = 0644;
	}
	if (IS("mode_ignored") && (flags & O_CREAT))
		mode = 0666;
	if (IS("empty_is_dot") && path[0] == '\0')
		path = ".";
	if (IS("excl_follows_symlink") && (flags & O_CREAT) &&
	    (flags & O_EXCL) && lstat(path, &st) == 0 && S_ISLNK(st.st_mode))
		flags &= ~O_EXCL;
	if (IS("excl_truncates") && (flags & O_CREAT) && (flags & O_EXCL)) {
		int t = real_open()(path, O_WRONLY | O_TRUNC);
		if (t >= 0)
			(void)close(t);
	}
	if (IS("creat_makes_dir") && (flags & O_CREAT) &&
	    lstat(path, &st) != 0) {
		if (mkdir(path, mode) != 0)
			return -1;
		return real_open()(path, O_RDONLY);
	}

	{
		/* State some outcome faults need, taken before the call. */
		static char dir[4096];
		const char *s = strrchr(path, '/');
		size_t n = s ? (size_t)(s - path) : 0;
		if (n == 0 || n >= sizeof dir)
			strcpy(dir, s == path ? "/" : ".");
		else {
			memcpy(dir, path, n);
			dir[n] = '\0';
		}
		existed = lstat(path, &st) == 0;
		have_parent = stat(dir, &pst) == 0;
		parent = dir;
		have_old = existed && stat(path, &old) == 0;
	}
	fd = real_open()(path, flags, mode);
	e = errno;

	/* Faults applied to the outcome. */
	if (fd < 0) {
		if (IS("eexist_as_eacces") && e == EEXIST)
			e = EACCES;
		if (IS("enoent_as_eacces") && e == ENOENT)
			e = EACCES;
		if (IS("enotdir_as_enoent") && e == ENOTDIR)
			e = ENOENT;
		if (IS("eisdir_as_eacces") && e == EISDIR)
			e = EACCES;
		if (IS("enametoolong_as_enoent") && e == ENAMETOOLONG)
			e = ENOENT;
		if (IS("enxio_as_eagain") && e == ENXIO)
			e = EAGAIN;
		if (IS("eintr_as_eagain") && e == EINTR)
			e = EAGAIN;
		if (IS("eloop_as_enoent") && e == ELOOP)
			e = ENOENT;
		if (IS("emfile_as_enfile") && e == EMFILE)
			e = ENFILE;
		errno = e;
		return -1;
	}
	if (IS("after_highest")) {
		/* Hand out one above the highest open descriptor instead. */
		int hi = fd, i, n;
		for (i = fd + 1; i < 1024; i++)
			if (fcntl(i, F_GETFD) != -1)
				hi = i;
		if (hi > fd) {
			n = fcntl(fd, F_DUPFD, hi + 1);
			if (n >= 0) {
				(void)close(fd);
				fd = n;
			}
		}
	}
	if (IS("not_lowest")) {
		int n = fcntl(fd, F_DUPFD, fd + 1);
		if (n >= 0) {
			(void)close(fd);
			fd = n;
		}
	}
	if (IS("mode_ignores_umask") && (flags & O_CREAT))
		(void)fchmod(fd, mode & 07777);
	if (IS("trunc_resets_mode") && (flags & O_TRUNC))
		(void)fchmod(fd, 0644);
	if (IS("offset_at_end"))
		(void)lseek(fd, 0, SEEK_END);
	if (IS("mode0_reopens_rdonly") && (flags & O_CREAT) && !existed &&
	    (mode & 0777) == 0) {
		/* Applies the new mode's permissions to the creating open. */
		int r = real_open()(path, O_RDONLY);
		(void)close(fd);
		fd = r;
	}
	if (IS("reuses_description")) {
		/* A second open of a file still open returns the old description. */
		static int last = -1;
		static dev_t ldev;
		static ino_t lino;
		struct stat ns;
		if (fstat(fd, &ns) == 0) {
			if (last >= 0 && fcntl(last, F_GETFD) != -1 &&
			    ns.st_dev == ldev && ns.st_ino == lino) {
				int d = dup(last);
				if (d >= 0) {
					(void)close(fd);
					return d;
				}
			}
			last = fd;
			ldev = ns.st_dev;
			lino = ns.st_ino;
		}
	}
	if (IS("creat_old_times") && (flags & O_CREAT) && !existed) {
		struct timespec ts[2] = { { 0, 0 }, { 0, 0 } };
		(void)futimens(fd, ts);
	}
	if (IS("creat_restores_parent_times") && (flags & O_CREAT) &&
	    !existed && have_parent) {
		struct timespec ts[2];
		ts[0] = pst.st_atim;
		ts[1] = pst.st_mtim;
		(void)utimensat(AT_FDCWD, parent, ts, 0);
	}
	if (IS("trunc_keeps_times") && (flags & O_TRUNC) && have_old) {
		struct timespec ts[2];
		ts[0] = old.st_atim;
		ts[1] = old.st_mtim;
		(void)futimens(fd, ts);
	}
	if (IS("creat_wrong_owner") && (flags & O_CREAT) && !existed) {
		if (fchown(fd, geteuid() + 1, (gid_t)-1) != 0) {
			(void)close(fd);
			errno = EACCES;
			return -1;
		}
	}
#undef IS
	return fd;
}

int openat(int dirfd, const char *path, int flags, ...)
{
	mode_t mode = 0;
	const char *m;
	int fd;

	if (flags & O_CREAT) {
		va_list ap;
		va_start(ap, flags);
		mode = (mode_t)va_arg(ap, int);
		va_end(ap);
	}
	m = mutant();
	if (m == NULL)
		return real_openat()(dirfd, path, flags, mode);

#define IS(name) (strcmp(m, name) == 0)
	if (IS("at_ignores_fd"))
		return real_open()(path, flags, mode);
	if (IS("at_fdcwd_ebadf") && dirfd == AT_FDCWD) {
		errno = EBADF;
		return -1;
	}
	if (IS("at_abs_uses_fd") && path[0] == '/') {
		while (path[0] == '/')
			path++;
		if (path[0] == '\0')
			path = ".";
	}
	fd = real_openat()(dirfd, path, flags, mode);
	if (fd >= 0 && IS("at_mode_ignores_umask") && (flags & O_CREAT))
		(void)fchmod(fd, mode & 07777);
#undef IS
	return fd;
}
