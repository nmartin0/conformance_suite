/*
 * pc.c - C test helper for posixcheck. See pc.h.
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * The helper deliberately uses only open/read/write/close/fstat/lstat and
 * friends, and writes results with write(2) so that a crash cannot lose a
 * buffered result line.
 */
#include "pc.h"

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

enum { PC_SETUP, PC_TEST };

static const char *pc_id = "?";
static int pc_phase = PC_SETUP;
static int pc_rfd = 1;          /* result file descriptor */
static pid_t pc_pid;            /* process that called pc_begin */
static int pc_began;

/* ---------- output ---------- */

static void pc_write_all(int fd, const char *s, size_t n)
{
	while (n > 0) {
		ssize_t w = write(fd, s, n);
		if (w < 0) {
			if (errno == EINTR)
				continue;
			return;
		}
		s += w;
		n -= (size_t)w;
	}
}

/* One line, with tabs and newlines in the note replaced by spaces. */
static void pc_line(const char *a, const char *b, const char *c)
{
	char buf[1024];
	size_t i;
	int n = snprintf(buf, sizeof buf - 1, "%s%s%s%s%s", a,
	    b ? " " : "", b ? b : "", c ? " " : "", c ? c : "");
	if (n < 0)
		return;
	if ((size_t)n > sizeof buf - 2)
		n = (int)(sizeof buf - 2);
	for (i = 0; i < (size_t)n; i++)
		if (buf[i] == '\n' || buf[i] == '\t' || buf[i] == '\r')
			buf[i] = ' ';
	buf[n++] = '\n';
	pc_write_all(pc_rfd, buf, (size_t)n);
}

/*
 * Mutation shims (used only in CI) act only while PC_PHASE is "test".
 * Helper internals switch it to "internal" so that a planted fault in,
 * say, open() cannot break the helper's own bookkeeping.
 */
static void pc_set_phase_env(const char *v)
{
	(void)setenv("PC_PHASE", v, 1);
}

static void pc_internal_enter(void)
{
	pc_set_phase_env("internal");
}

static void pc_internal_leave(void)
{
	pc_set_phase_env(pc_phase == PC_TEST ? "test" : "setup");
}

_Noreturn static void pc_report(const char *code, const char *note)
{
	char rest[900];

	pc_internal_enter();	/* planted faults in exit() must not hit the report */
	if (!pc_began) {
		fprintf(stderr, "posixcheck: result before pc_begin()\n");
		_exit(2);
	}
	if (getpid() != pc_pid) {
		/* A forked child reports to its parent by exit status only. */
		fprintf(stderr, "posixcheck: child %s: %s\n", code, note);
		_exit(strcmp(code, "PASS") == 0 ? 0 : 1);
	}
	(void)snprintf(rest, sizeof rest, "%s %s", pc_id, note);
	pc_line("RESULT", code, rest);
	exit(0);
}

void pc_result_raw(const char *code, const char *note)
{
	char rest[900];
	pc_internal_enter();
	(void)snprintf(rest, sizeof rest, "%s %s", pc_id, note ? note : "");
	pc_line("RESULT", code, rest);
	pc_internal_leave();
}

#define PC_RESULT_FN(name, code)                                         \
	_Noreturn void name(const char *fmt, ...)                        \
	{                                                                \
		va_list ap;                                              \
		char note[768] = "";                                     \
		pc_internal_enter();                                     \
		va_start(ap, fmt);                                       \
		if (fmt)                                                 \
			(void)vsnprintf(note, sizeof note, fmt, ap);     \
		va_end(ap);                                              \
		pc_report(code, note);                                   \
	}

PC_RESULT_FN(pc_unresolved, "UNRESOLVED")
PC_RESULT_FN(pc_notinuse, "NOTINUSE")
PC_RESULT_FN(pc_unsupported, "UNSUPPORTED")

_Noreturn void pc_pass(void)
{
	pc_internal_enter();
	pc_report("PASS", "");
}

/* FAIL in the test phase; UNRESOLVED (with "setup:") in the setup phase. */
_Noreturn void pc_fail(const char *fmt, ...)
{
	va_list ap;
	char note[768] = "";
	char note2[800];
	int test = (pc_phase == PC_TEST);

	pc_internal_enter();
	va_start(ap, fmt);
	if (fmt)
		(void)vsnprintf(note, sizeof note, fmt, ap);
	va_end(ap);
	if (test)
		pc_report("FAIL", note);
	(void)snprintf(note2, sizeof note2, "setup: %s", note);
	pc_report("UNRESOLVED", note2);
}

void pc_note(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	fputs("note: ", stderr);
	vfprintf(stderr, fmt, ap);
	fputc('\n', stderr);
	fflush(stderr);
	va_end(ap);
}

/* ---------- phases ---------- */

void pc_begin(const char *assertion)
{
	const char *rf = getenv("PC_RESULT");

	pc_id = assertion;
	pc_pid = getpid();
	pc_phase = PC_SETUP;
	pc_began = 1;
	pc_internal_enter();
	if (rf && *rf) {
		int fd = open(rf, O_WRONLY | O_APPEND | O_CREAT | O_CLOEXEC,
		    0644);
		if (fd < 0) {
			fprintf(stderr, "posixcheck: cannot open %s\n", rf);
			_exit(2);
		}
		pc_rfd = fd;
	}
	pc_line("PHASE", "setup", NULL);
	pc_internal_leave();
}

void pc_test(void)
{
	pc_internal_enter();
	pc_phase = PC_TEST;
	pc_line("PHASE", "test", NULL);
	fflush(NULL);
	pc_internal_leave();
}

/* ---------- checks ---------- */

void pc_check(int cond, const char *fmt, ...)
{
	va_list ap;
	char note[768] = "";

	if (cond)
		return;
	va_start(ap, fmt);
	if (fmt)
		(void)vsnprintf(note, sizeof note, fmt, ap);
	va_end(ap);
	pc_fail("%s", note);
}

void pc_expect_err(long r, int err, const char *what)
{
	int e = errno;
	if (r != -1)
		pc_fail("%s: returned %ld, expected -1 with %s", what, r,
		    pc_errname(err));
	if (e != err)
		pc_fail("%s: errno %s, expected %s", what, pc_errname(e),
		    pc_errname(err));
}

void pc_expect_ok(long r, const char *what)
{
	int e = errno;
	if (r == -1)
		pc_fail("%s: failed with %s", what, pc_errname(e));
}

static const struct {
	int value;
	const char *name;
} pc_errtab[] = {
#include "pc_errnames.h"
	{ 0, "0" }
};

const char *pc_errname(int e)
{
	static char unknown[32];
	size_t i;

	for (i = 0; i < sizeof pc_errtab / sizeof pc_errtab[0]; i++)
		if (pc_errtab[i].value == e)
			return pc_errtab[i].name;
	(void)snprintf(unknown, sizeof unknown, "E?%d", e);
	return unknown;
}

/* ---------- setup helpers ---------- */

_Noreturn static void pc_setup_broke(const char *what, const char *path)
{
	int e = errno;
	pc_unresolved("helper %s(%s) failed: %s", what, path, pc_errname(e));
}

void pc_make_file(const char *path, const char *contents)
{
	int fd;
	size_t n = strlen(contents);

	pc_internal_enter();
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		pc_setup_broke("make_file", path);
	while (n > 0) {
		ssize_t w = write(fd, contents, n);
		if (w < 0) {
			if (errno == EINTR)
				continue;
			pc_setup_broke("make_file/write", path);
		}
		contents += w;
		n -= (size_t)w;
	}
	if (close(fd) != 0)
		pc_setup_broke("make_file/close", path);
	pc_internal_leave();
}

void pc_make_dir(const char *path)
{
	pc_internal_enter();
	if (mkdir(path, 0755) != 0)
		pc_setup_broke("make_dir", path);
	pc_internal_leave();
}

void pc_make_symlink(const char *target, const char *path)
{
	pc_internal_enter();
	if (symlink(target, path) != 0)
		pc_setup_broke("make_symlink", path);
	pc_internal_leave();
}

int pc_lowest_free_fd(void)
{
	long max = sysconf(_SC_OPEN_MAX);
	int fd;

	pc_internal_enter();
	if (max < 0 || max > 65536)
		max = 65536;
	for (fd = 0; fd < max; fd++) {
		if (fcntl(fd, F_GETFD) == -1 && errno == EBADF) {
			pc_internal_leave();
			return fd;
		}
	}
	pc_unresolved("helper lowest_free_fd: no free descriptor below %ld",
	    max);
}

long pc_file_size(const char *path)
{
	struct stat st;
	pc_internal_enter();
	if (lstat(path, &st) != 0)
		pc_setup_broke("file_size", path);
	pc_internal_leave();
	return (long)st.st_size;
}

unsigned pc_file_mode(const char *path)
{
	struct stat st;
	pc_internal_enter();
	if (lstat(path, &st) != 0)
		pc_setup_broke("file_mode", path);
	pc_internal_leave();
	return (unsigned)(st.st_mode & 07777);
}

int pc_file_equals(const char *path, const char *s)
{
	char buf[4096];
	size_t want = strlen(s), got = 0;
	int fd, same = 1;

	pc_internal_enter();
	fd = open(path, O_RDONLY);
	if (fd < 0)
		pc_setup_broke("file_equals", path);
	for (;;) {
		ssize_t r = read(fd, buf, sizeof buf);
		if (r < 0) {
			if (errno == EINTR)
				continue;
			pc_setup_broke("file_equals/read", path);
		}
		if (r == 0)
			break;
		if (got + (size_t)r > want ||
		    memcmp(buf, s + got, (size_t)r) != 0)
			same = 0;
		got += (size_t)r;
	}
	(void)close(fd);
	pc_internal_leave();
	return same && got == want;
}

void pc_require_space(void)
{
	int fd;

	pc_internal_enter();
	fd = open(".pcspace", O_WRONLY | O_CREAT | O_EXCL, 0600);
	if (fd < 0) {
		if (errno == ENOSPC)
			pc_unresolved("the file system is full (ENOSPC)");
		pc_setup_broke("require_space", ".pcspace");
	}
	if (write(fd, "x", 1) != 1) {
		if (errno == ENOSPC)
			pc_unresolved("the file system is full (ENOSPC)");
		pc_setup_broke("require_space/write", ".pcspace");
	}
	(void)close(fd);
	if (unlink(".pcspace") != 0)
		pc_setup_broke("require_space/unlink", ".pcspace");
	pc_internal_leave();
}
