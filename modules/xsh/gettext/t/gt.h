/*
 * gt.h - helpers for the gettext tests: make a messages object with the
 * msgfmt utility, and pick a locale whose name is not the POSIX locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * The messages object format is unspecified, so every object the tests
 * use is built by msgfmt from a source file; if msgfmt cannot be run the
 * test is UNRESOLVED. Only ASCII text is translated, so no code set
 * conversion is involved.
 */
#ifndef GT_H
#define GT_H

#include "pc.h"
#include <errno.h>
#include <iconv.h>
#include <langinfo.h>
#include <libintl.h>
#include <limits.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

/* The names used in the source: one singular and one plural message. */
#define GT_ONE "pc one"
#define GT_ONE_T "pc uno"
#define GT_FILE "pc file"
#define GT_FILES "pc files"
#define GT_PL0 "pc fil1"
#define GT_PL1 "pc fil2"
#define GT_PL2 "pc filN"

static inline const char *gt_cwd(void)
{
	static char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof cwd) == NULL)
		pc_unresolved("getcwd failed");
	return cwd;
}

/* Make dir/loc/LC_MESSAGES/domain.mo with msgfmt; returns nothing, UNRESOLVED on failure. */
static inline void gt_make_mo(const char *dir, const char *loc, const char *domain, const char *codeset)
{
	char p[1200], cmd[2600], src[2200];

	(void)mkdir(dir, 0777);
	(void)snprintf(p, sizeof p, "%s/%s", dir, loc);
	(void)mkdir(p, 0777);
	(void)snprintf(p, sizeof p, "%s/%s/LC_MESSAGES", dir, loc);
	(void)mkdir(p, 0777);
	(void)snprintf(src, sizeof src,
	    "msgid \"\"\n"
	    "msgstr \"\"\n"
	    "\"Content-Type: text/plain; charset=%s\\n\"\n"
	    "\"Plural-Forms: nplurals=3; plural=n==1?0:n==2?1:2;\\n\"\n"
	    "\n"
	    "msgid \"" GT_ONE "\"\n"
	    "msgstr \"" GT_ONE_T "\"\n"
	    "\n"
	    "msgid \"" GT_FILE "\"\n"
	    "msgid_plural \"" GT_FILES "\"\n"
	    "msgstr[0] \"" GT_PL0 "\"\n"
	    "msgstr[1] \"" GT_PL1 "\"\n"
	    "msgstr[2] \"" GT_PL2 "\"\n", codeset);
	(void)snprintf(p, sizeof p, "%s.po", domain);
	pc_make_file(p, src);
	(void)snprintf(cmd, sizeof cmd, "msgfmt -o '%s/%s/LC_MESSAGES/%s.mo' '%s' >/dev/null 2>&1",
	    dir, loc, domain, p);
	if (system(cmd) != 0)
		pc_unresolved("the msgfmt utility could not build the test messages object");
}

/*
 * Like gt_make_mo() but with the one message GT_CAFE whose translation is the
 * given bytes, in the code set named in the header.
 */
#define GT_CAFE "pc cafe"

static inline void gt_make_mo_cafe(const char *dir, const char *loc, const char *domain,
    const char *charset, const char *msgstr)
{
	char p[1200], cmd[2600], src[800];

	(void)mkdir(dir, 0777);
	(void)snprintf(p, sizeof p, "%s/%s", dir, loc);
	(void)mkdir(p, 0777);
	(void)snprintf(p, sizeof p, "%s/%s/LC_MESSAGES", dir, loc);
	(void)mkdir(p, 0777);
	(void)snprintf(src, sizeof src,
	    "msgid \"\"\n"
	    "msgstr \"\"\n"
	    "\"Content-Type: text/plain; charset=%s\\n\"\n"
	    "\n"
	    "msgid \"" GT_CAFE "\"\n"
	    "msgstr \"%s\"\n", charset, msgstr);
	(void)snprintf(p, sizeof p, "%s.po", domain);
	pc_make_file(p, src);
	(void)snprintf(cmd, sizeof cmd, "msgfmt -o '%s/%s/LC_MESSAGES/%s.mo' '%s' >/dev/null 2>&1",
	    dir, loc, domain, p);
	if (system(cmd) != 0)
		pc_unresolved("the msgfmt utility could not build the test messages object");
}

/*
 * Switch the process to an installed locale with a UTF-8 code set whose
 * name is not the POSIX or C locale (nor looks like one), and copy the
 * LC_MESSAGES name into name. UNRESOLVED when there is none. The locale
 * stays set.
 */
static inline void gt_pick_locale(char *name, size_t sz, char *codeset, size_t csz)
{
	const char *f, *ln;

	f = pc_find_locale("UTF-8", 0);
	if (f == NULL)
		pc_unresolved("no installed locale with a UTF-8 code set");
	if (setlocale(LC_ALL, f) == NULL)
		pc_unresolved("setlocale to the found locale failed");
	ln = setlocale(LC_MESSAGES, NULL);
	if (ln == NULL || strlen(ln) >= sz || ln[0] == '\0' || strchr(ln, '/') != NULL ||
	    strcmp(ln, "C") == 0 || strcmp(ln, "POSIX") == 0 || strncmp(ln, "C.", 2) == 0 ||
	    strncmp(ln, "POSIX", 5) == 0)
		pc_unresolved("the only locale found has a name that may stand for the POSIX locale");
	(void)snprintf(name, sz, "%s", ln);
	(void)snprintf(codeset, csz, "%s", nl_langinfo(CODESET));
}

#endif
