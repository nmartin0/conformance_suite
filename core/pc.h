/*
 * pc.h - C test helper for posixcheck.
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Every C test includes this header and follows the same shape:
 *
 *     pc_begin("XSH.open.003");   setup phase: problems are UNRESOLVED
 *     ... prepare files ...
 *     pc_test();                  test phase: problems are FAIL
 *     ... call the interface, check with pc_check / pc_expect_err ...
 *     pc_pass();
 *
 * The helper writes "PHASE ..." and one "RESULT <code> <id> <note>" line to
 * the file named by $PC_RESULT (or to standard output when it is unset, so a
 * test can be run by hand). The driver turns that into a log line.
 *
 * Tests are compiled by the driver with -D_POSIX_C_SOURCE=202405L, or with
 * -D_XOPEN_SOURCE=800 for XSI tests. Do not define feature macros yourself.
 */
#ifndef PC_H
#define PC_H

#if !defined(_POSIX_C_SOURCE) && !defined(_XOPEN_SOURCE)
#error "compile with -D_POSIX_C_SOURCE=202405L (posixcheck.sh does this)"
#endif

#include <stddef.h>

/* Phases and results. Each result function writes the result and exits. */
void pc_begin(const char *assertion);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *fmt, ...);
_Noreturn void pc_unresolved(const char *fmt, ...);
_Noreturn void pc_notinuse(const char *fmt, ...);
_Noreturn void pc_unsupported(const char *fmt, ...);

/* Diagnostics to standard error, kept in the test's output file. */
void pc_note(const char *fmt, ...);

/*
 * Checks. In the setup phase a failed check is UNRESOLVED; in the test
 * phase it is FAIL. The first failed check ends the test.
 */
void pc_check(int cond, const char *fmt, ...);
/* r must be -1 and errno must be err. */
void pc_expect_err(long r, int err, const char *what);
/* r must not be -1. */
void pc_expect_ok(long r, const char *what);

/* Symbolic name of an errno value ("EEXIST"), or "E?<n>" if unknown. */
const char *pc_errname(int e);

/*
 * Setup helpers. They run with mutants disabled, and any failure inside
 * them is UNRESOLVED, whichever phase the test is in.
 */
void pc_make_file(const char *path, const char *contents);
void pc_make_dir(const char *path);
void pc_make_symlink(const char *target, const char *path);
int pc_lowest_free_fd(void);
long pc_file_size(const char *path);
/* 1 if the file's contents equal s exactly, 0 if not. */
int pc_file_equals(const char *path, const char *s);
/* Mode bits (st_mode & 07777) of path. */
unsigned pc_file_mode(const char *path);
/*
 * Make sure a new file can be created in the current directory; if the
 * file system is full the test is UNRESOLVED rather than a false FAIL.
 */
void pc_require_space(void);

/*
 * Low level: write one RESULT line without exiting. Only the driver's
 * self-tests use this, to produce malformed results on purpose.
 */
void pc_result_raw(const char *code, const char *note);

/*
 * Drops supplementary groups where the system has setgroups() (not POSIX):
 * 0 on success, -1 if not done (core/pcgroups.c). Callers still check with
 * getgroups().
 */
int pc_drop_groups(void);

#endif
