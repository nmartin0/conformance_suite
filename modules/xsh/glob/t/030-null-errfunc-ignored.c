/*
 * With a null errfunc and no GLOB_ERR the failure is ignored.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.glob.030
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static int ef_ret, ef_good;

static int errf(const char *path, int e)
{
	if (path != NULL && strncmp(path, "locked", 6) == 0 && e == EACCES)
		ef_good = 1;
	return ef_ret;
}

/* Directory a (readable, holds f) and locked (empty, no access for anyone: left empty so a run without privilege can remove it). */
static void arrange(void)
{
	pc_make_dir("a");
	pc_make_file("a/f", "");
	pc_make_dir("locked");
	if (chmod("a", 0755) != 0 || chmod(".", 0755) != 0 || chmod("locked", 0) != 0)
		pc_unresolved("chmod failed: %s", pc_errname(errno));
}

/* The child runs without privilege; fn gives its exit status (0 to 89). */
static int unprivileged(int (*fn)(void))
{
	pid_t kid;
	int st;

	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		if (geteuid() == 0 && (setgid(65534) != 0 || setuid(65534) != 0))
			_exit(99);
		if (geteuid() == 0)
			_exit(99);
		if (opendir("locked") != NULL)
			_exit(97);
		_exit(fn());
	}
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (!WIFEXITED(st) || WEXITSTATUS(st) >= 90)
		pc_unresolved("the unprivileged child could not run or still has access (status %#x)", (unsigned)st);
	return WEXITSTATUS(st);
}

/* 1 if g holds exactly a/f. */
static int only_af(const glob_t *g)
{
	return g->gl_pathc == 1 && g->gl_pathv[0] != NULL && strcmp(g->gl_pathv[0], "a/f") == 0;
}

static int child(void)
{
	glob_t g;
	int r;

	r = glob("*/*", 0, NULL, &g);
	if (r != 0)
		return 1;
	if (!only_af(&g))
		return 2;
	return 0;
}

static const char *const msgs[] = {
	"ok",
	"glob() did not return 0",
	"the result is not exactly a/f",
};

int main(void)
{
	int st;

	pc_begin("XSH.glob.030");
	arrange();
	pc_test();
	st = unprivileged(child);
	pc_check(st == 0, "%s", st <= 2 ? msgs[st] : "unexpected child status");
	pc_pass();
}
