/*
 * ldef.h - helpers for tests that create a locale with the localedef
 * utility and then use it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Creating locales is optional (POSIX2_LOCALEDEF): ld_need() reports
 * NOTINUSE when it is not claimed. A source the test wrote that localedef
 * refuses, or a locale that cannot then be selected, is UNRESOLVED: the
 * default character mapping, and whether users may supply charmap files,
 * are left to the implementation. Only what the created locale then does
 * is judged. Every locale and file is made in the test's own directory.
 */
#ifndef LDEF_H
#define LDEF_H

#include "pc.h"
#include "pcchars.h"
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define LD_CP(cat) cat "\ncopy \"POSIX\"\nEND " cat "\n"
#define LD_CP_CTYPE LD_CP("LC_CTYPE")
#define LD_CP_COLLATE LD_CP("LC_COLLATE")
#define LD_CP_MONETARY LD_CP("LC_MONETARY")
#define LD_CP_NUMERIC LD_CP("LC_NUMERIC")
#define LD_CP_TIME LD_CP("LC_TIME")
#define LD_CP_MESSAGES LD_CP("LC_MESSAGES")
#define LD_CP_ALL LD_CP_CTYPE LD_CP_COLLATE LD_CP_MONETARY LD_CP_NUMERIC LD_CP_TIME LD_CP_MESSAGES

#define LD_HDR "<code_set_name> PCTEST\n<mb_cur_max> 1\n<mb_cur_min> 1\n"

static void ld_need(void)
{
	long v = sysconf(_SC_2_LOCALEDEF);

	if (v <= 0)
		pc_notinuse("creating locales is not claimed (_SC_2_LOCALEDEF is %ld)", v);
}

static const char *const ld_vars[] = { "LANG", "LC_ALL", "LC_COLLATE", "LC_CTYPE", "LC_MESSAGES", "LC_MONETARY",
    "LC_NUMERIC", "LC_TIME", NULL };

static void ld_unset_all(void)
{
	int i;

	for (i = 0; ld_vars[i] != NULL; i++)
		(void)unsetenv(ld_vars[i]);
}

static char ld_cwd[4096];

static const char *ld_dir(void)
{
	if (ld_cwd[0] == '\0') {
		if (getcwd(ld_cwd, sizeof ld_cwd) == NULL)
			pc_unresolved("getcwd failed: %s", pc_errname(errno));
		if (ld_cwd[0] != '/' || strchr(ld_cwd, '\'') != NULL)
			pc_unresolved("the working directory name is unsuitable for a pathname operand");
	}
	return ld_cwd;
}

/* Absolute pathname of a file or locale of the given name in the working directory. */
static void ld_path(char *out, size_t n, const char *name)
{
	(void)snprintf(out, n, "%s/%s", ld_dir(), name);
}

/*
 * Write a charmap file: the declarations in header, a line for every
 * portable character (and its second name) with the encodings of the
 * character constants written with escape character esc, then extra mapping
 * lines, END CHARMAP and the text that follows it. With text, each portable
 * line also carries a comment.
 */
static void ld_charmap(const char *name, const char *header, char esc, const char *extra, const char *tail, int text)
{
	static char buf[30000];
	size_t len = 0;
	char nb[4];
	int i, w;

	w = snprintf(buf, sizeof buf, "%sCHARMAP\n", header);
	len = (size_t)w;
	for (i = 0; i < PCC_N; i++) {
		unsigned u = (unsigned char)pcc_all[i];
		const char *al = pcc_alias(pcc_all[i]);

		w = snprintf(buf + len, sizeof buf - len, "<%s> %cx%02X%s\n", pcc_sym(pcc_all[i], nb), esc, u,
		    text ? " portable character" : "");
		len += (size_t)w;
		if (al != NULL) {
			w = snprintf(buf + len, sizeof buf - len, "<%s> %cx%02X\n", al, esc, u);
			len += (size_t)w;
		}
	}
	w = snprintf(buf + len, sizeof buf - len, "%sEND CHARMAP\n%s", extra, tail);
	len += (size_t)w;
	if (len >= sizeof buf)
		pc_unresolved("charmap text too long");
	pc_make_file(name, buf);
}

/* Run localedef; the exit status, or 255 if it could not be run. */
static int ld_run(const char *opts, const char *cmap, const char *src, const char *loc)
{
	char cmd[8192];
	char cpath[4200] = "";
	int r;

	if (cmap != NULL)
		(void)snprintf(cpath, sizeof cpath, "-f '%s/%s' ", ld_dir(), cmap);
	(void)snprintf(cmd, sizeof cmd, "localedef %s %s-i '%s/%s' '%s' >ld.out 2>ld.err </dev/null", opts, cpath, ld_dir(), src,
	    loc);
	ld_unset_all();
	(void)setenv("LC_ALL", "C", 1);
	r = system(cmd);
	ld_unset_all();
	if (r == -1 || !WIFEXITED(r))
		return 255;
	return WEXITSTATUS(r);
}

static void ld_errnote(void)
{
	char line[300];
	FILE *f = fopen("ld.err", "r");

	if (f != NULL) {
		if (fgets(line, sizeof line, f) != NULL)
			pc_note("localedef said: %s", line);
		(void)fclose(f);
	}
}

/* Write <tag>.src from text and make the locale <cwd>/<tag>.loc; loc receives its name. */
static void ld_ok(const char *tag, const char *cmap, const char *text, char *loc, size_t locn)
{
	char src[200], lname[200];
	int rc;

	(void)snprintf(src, sizeof src, "%s.src", tag);
	(void)snprintf(lname, sizeof lname, "%s.loc", tag);
	pc_make_file(src, text);
	ld_path(loc, locn, lname);
	rc = ld_run("-c", cmap, src, loc);
	if (rc != 0 && rc != 1) {
		ld_errnote();
		pc_unresolved("localedef refused the test source %s (status %d)", src, rc);
	}
	if (access(loc, F_OK) != 0)
		pc_unresolved("localedef said it succeeded but nothing exists at %s", loc);
}

/* Make LC_ALL name the locale and select it for the whole program. */
static void ld_load(const char *loc)
{
	ld_unset_all();
	if (setenv("LC_ALL", loc, 1) != 0)
		pc_unresolved("setenv failed");
	if (setlocale(LC_ALL, "") == NULL)
		pc_unresolved("the locale localedef created cannot be selected through LC_ALL");
}

#endif
