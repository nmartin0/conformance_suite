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

#define LD_COPY(cat) cat "\ncopy \"POSIX\"\nEND " cat "\n"
#define LD_COPY_ALL LD_COPY("LC_CTYPE") LD_COPY("LC_COLLATE") LD_COPY("LC_MONETARY") LD_COPY("LC_NUMERIC") LD_COPY("LC_TIME") LD_COPY("LC_MESSAGES")

/*
 * Categories for the parts of a locale a test does not look at: each is
 * written out (the values of the POSIX locale, for the portable characters
 * only), so that no test depends on the copy statement; the tests about the
 * copy statement use LD_COPY_ALL. The symbolic names are those of the charmap
 * written by ld_charmap().
 */
#define LD_CP_CTYPE \
    "LC_CTYPE\n" \
    "upper <A>;<B>;<C>;<D>;<E>;<F>;<G>;<H>;<I>;<J>;<K>;<L>;<M>;<N>;<O>;<P>;<Q>;<R>;<S>;<T>;<U>;<V>;<W>;<X>;<Y>;<Z>\n" \
    "lower <a>;<b>;<c>;<d>;<e>;<f>;<g>;<h>;<i>;<j>;<k>;<l>;<m>;<n>;<o>;<p>;<q>;<r>;<s>;<t>;<u>;<v>;<w>;<x>;<y>;<z>\n" \
    "digit <zero>;<one>;<two>;<three>;<four>;<five>;<six>;<seven>;<eight>;<nine>\n" \
    "space <space>;<tab>;<newline>;<vertical-tab>;<form-feed>;<carriage-return>\n" \
    "cntrl <NUL>;<alert>;<backspace>;<tab>;<newline>;<vertical-tab>;<form-feed>;<carriage-return>\n" \
    "punct <exclamation-mark>;<quotation-mark>;<number-sign>;<dollar-sign>;<percent-sign>;<ampersand>;<apostrophe>;<left-parenthesis>;<right-parenthesis>;<asterisk>;<plus-sign>;<comma>;<hyphen-minus>;<period>;<slash>;<colon>;<semicolon>;<less-than-sign>;<equals-sign>;<greater-than-sign>;<question-mark>;<commercial-at>;<left-square-bracket>;<backslash>;<right-square-bracket>;<circumflex>;<underscore>;<grave-accent>;<left-curly-bracket>;<vertical-line>;<right-curly-bracket>;<tilde>\n" \
    "xdigit <zero>;<one>;<two>;<three>;<four>;<five>;<six>;<seven>;<eight>;<nine>;<A>;<B>;<C>;<D>;<E>;<F>;<a>;<b>;<c>;<d>;<e>;<f>\n" \
    "blank <space>;<tab>\n" \
    "toupper (<a>,<A>);(<b>,<B>);(<c>,<C>);(<d>,<D>);(<e>,<E>);(<f>,<F>);(<g>,<G>);(<h>,<H>);(<i>,<I>);(<j>,<J>);(<k>,<K>);(<l>,<L>);(<m>,<M>);(<n>,<N>);(<o>,<O>);(<p>,<P>);(<q>,<Q>);(<r>,<R>);(<s>,<S>);(<t>,<T>);(<u>,<U>);(<v>,<V>);(<w>,<W>);(<x>,<X>);(<y>,<Y>);(<z>,<Z>)\n" \
    "tolower (<A>,<a>);(<B>,<b>);(<C>,<c>);(<D>,<d>);(<E>,<e>);(<F>,<f>);(<G>,<g>);(<H>,<h>);(<I>,<i>);(<J>,<j>);(<K>,<k>);(<L>,<l>);(<M>,<m>);(<N>,<n>);(<O>,<o>);(<P>,<p>);(<Q>,<q>);(<R>,<r>);(<S>,<s>);(<T>,<t>);(<U>,<u>);(<V>,<v>);(<W>,<w>);(<X>,<x>);(<Y>,<y>);(<Z>,<z>)\n" \
    "END LC_CTYPE\n"
#define LD_CP_COLLATE \
    "LC_COLLATE\n" \
    "order_start forward\n" \
    "<NUL>\n" \
    "<alert>\n" \
    "<backspace>\n" \
    "<tab>\n" \
    "<newline>\n" \
    "<vertical-tab>\n" \
    "<form-feed>\n" \
    "<carriage-return>\n" \
    "<space>\n" \
    "<exclamation-mark>\n" \
    "<quotation-mark>\n" \
    "<number-sign>\n" \
    "<dollar-sign>\n" \
    "<percent-sign>\n" \
    "<ampersand>\n" \
    "<apostrophe>\n" \
    "<left-parenthesis>\n" \
    "<right-parenthesis>\n" \
    "<asterisk>\n" \
    "<plus-sign>\n" \
    "<comma>\n" \
    "<hyphen-minus>\n" \
    "<period>\n" \
    "<slash>\n" \
    "<zero>\n" \
    "<one>\n" \
    "<two>\n" \
    "<three>\n" \
    "<four>\n" \
    "<five>\n" \
    "<six>\n" \
    "<seven>\n" \
    "<eight>\n" \
    "<nine>\n" \
    "<colon>\n" \
    "<semicolon>\n" \
    "<less-than-sign>\n" \
    "<equals-sign>\n" \
    "<greater-than-sign>\n" \
    "<question-mark>\n" \
    "<commercial-at>\n" \
    "<A>\n" \
    "<B>\n" \
    "<C>\n" \
    "<D>\n" \
    "<E>\n" \
    "<F>\n" \
    "<G>\n" \
    "<H>\n" \
    "<I>\n" \
    "<J>\n" \
    "<K>\n" \
    "<L>\n" \
    "<M>\n" \
    "<N>\n" \
    "<O>\n" \
    "<P>\n" \
    "<Q>\n" \
    "<R>\n" \
    "<S>\n" \
    "<T>\n" \
    "<U>\n" \
    "<V>\n" \
    "<W>\n" \
    "<X>\n" \
    "<Y>\n" \
    "<Z>\n" \
    "<left-square-bracket>\n" \
    "<backslash>\n" \
    "<right-square-bracket>\n" \
    "<circumflex>\n" \
    "<underscore>\n" \
    "<grave-accent>\n" \
    "<a>\n" \
    "<b>\n" \
    "<c>\n" \
    "<d>\n" \
    "<e>\n" \
    "<f>\n" \
    "<g>\n" \
    "<h>\n" \
    "<i>\n" \
    "<j>\n" \
    "<k>\n" \
    "<l>\n" \
    "<m>\n" \
    "<n>\n" \
    "<o>\n" \
    "<p>\n" \
    "<q>\n" \
    "<r>\n" \
    "<s>\n" \
    "<t>\n" \
    "<u>\n" \
    "<v>\n" \
    "<w>\n" \
    "<x>\n" \
    "<y>\n" \
    "<z>\n" \
    "<left-curly-bracket>\n" \
    "<vertical-line>\n" \
    "<right-curly-bracket>\n" \
    "<tilde>\n" \
    "order_end\n" \
    "END LC_COLLATE\n"
#define LD_CP_MONETARY \
    "LC_MONETARY\n" \
    "int_curr_symbol \"\"\n" \
    "currency_symbol \"\"\n" \
    "mon_decimal_point \"\"\n" \
    "mon_thousands_sep \"\"\n" \
    "mon_grouping -1\n" \
    "positive_sign \"\"\n" \
    "negative_sign \"\"\n" \
    "int_frac_digits -1\n" \
    "frac_digits -1\n" \
    "p_cs_precedes -1\n" \
    "p_sep_by_space -1\n" \
    "n_cs_precedes -1\n" \
    "n_sep_by_space -1\n" \
    "p_sign_posn -1\n" \
    "n_sign_posn -1\n" \
    "int_p_cs_precedes -1\n" \
    "int_p_sep_by_space -1\n" \
    "int_n_cs_precedes -1\n" \
    "int_n_sep_by_space -1\n" \
    "int_p_sign_posn -1\n" \
    "int_n_sign_posn -1\n" \
    "END LC_MONETARY\n"
#define LD_CP_NUMERIC \
    "LC_NUMERIC\n" \
    "decimal_point \"<period>\"\n" \
    "thousands_sep \"\"\n" \
    "grouping -1\n" \
    "END LC_NUMERIC\n"
#define LD_CP_TIME \
    "LC_TIME\n" \
    "abday \"<S><u><n>\";\"<M><o><n>\";\"<T><u><e>\";\"<W><e><d>\";\"<T><h><u>\";\"<F><r><i>\";\"<S><a><t>\"\n" \
    "day \"<S><u><n><d><a><y>\";\"<M><o><n><d><a><y>\";\"<T><u><e><s><d><a><y>\";\"<W><e><d><n><e><s><d><a><y>\";\"<T><h><u><r><s><d><a><y>\";\"<F><r><i><d><a><y>\";\"<S><a><t><u><r><d><a><y>\"\n" \
    "abmon \"<J><a><n>\";\"<F><e><b>\";\"<M><a><r>\";\"<A><p><r>\";\"<M><a><y>\";\"<J><u><n>\";\"<J><u><l>\";\"<A><u><g>\";\"<S><e><p>\";\"<O><c><t>\";\"<N><o><v>\";\"<D><e><c>\"\n" \
    "mon \"<J><a><n><u><a><r><y>\";\"<F><e><b><r><u><a><r><y>\";\"<M><a><r><c><h>\";\"<A><p><r><i><l>\";\"<M><a><y>\";\"<J><u><n><e>\";\"<J><u><l><y>\";\"<A><u><g><u><s><t>\";\"<S><e><p><t><e><m><b><e><r>\";\"<O><c><t><o><b><e><r>\";\"<N><o><v><e><m><b><e><r>\";\"<D><e><c><e><m><b><e><r>\"\n" \
    "d_t_fmt \"<percent-sign><a><space><percent-sign><b><space><percent-sign><e><space><percent-sign><H><colon><percent-sign><M><colon><percent-sign><S><space><percent-sign><Y>\"\n" \
    "d_fmt \"<percent-sign><m><slash><percent-sign><d><slash><percent-sign><y>\"\n" \
    "t_fmt \"<percent-sign><H><colon><percent-sign><M><colon><percent-sign><S>\"\n" \
    "am_pm \"<A><M>\";\"<P><M>\"\n" \
    "t_fmt_ampm \"<percent-sign><I><colon><percent-sign><M><colon><percent-sign><S><space><percent-sign><p>\"\n" \
    "END LC_TIME\n"
#define LD_CP_MESSAGES \
    "LC_MESSAGES\n" \
    "yesexpr \"<circumflex><left-square-bracket><y><Y><right-square-bracket>\"\n" \
    "noexpr \"<circumflex><left-square-bracket><n><N><right-square-bracket>\"\n" \
    "END LC_MESSAGES\n"
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
	int k;
	FILE *f = fopen("ld.err", "r");

	if (f != NULL) {
		for (k = 0; k < 4 && fgets(line, sizeof line, f) != NULL; k++)
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

/*
 * Like ld_ok() for a source whose every category is a copy of the POSIX
 * locale: that locale exists on every system, so a source that localedef
 * refuses for any reason other than limits or an unsupported code set
 * (status 2 or 3) is a FAIL of localedef, not a setup problem. Enters the
 * test phase before running localedef.
 */
static void ld_ok_copy(const char *tag, const char *cmap, const char *text, char *loc, size_t locn)
{
	char src[200], lname[200];
	int rc;

	(void)snprintf(src, sizeof src, "%s.src", tag);
	(void)snprintf(lname, sizeof lname, "%s.loc", tag);
	pc_make_file(src, text);
	ld_path(loc, locn, lname);
	pc_test();
	rc = ld_run("-c", cmap, src, loc);
	if (rc > 3 && rc != 255) {
		ld_errnote();
		pc_fail("localedef refused a source made only of copies of the POSIX locale (status %d)", rc);
	}
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
