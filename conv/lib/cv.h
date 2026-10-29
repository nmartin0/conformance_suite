/*
 * cv.h - helpers shared by the tests of the conventions suite.
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Include after "pc.h". Everything is static: a test is one translation unit.
 *
 * Overrides: a test that reads a file or directory outside its own private
 * directory takes the name from an environment variable PC_CONV_<NAME>
 * (PC_* variables are the only ones the driver passes on). conv/run.sh sets
 * the ones it can learn from the caller's environment; conv/neg/run.sh points
 * them at deliberately wrong fixtures to show that the checks can fail.
 */
#ifndef CV_H
#define CV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* Value of PC_CONV_<name>, or NULL when unset or empty. */
static const char *cv_env(const char *name)
{
	char key[64];
	const char *v;

	(void)snprintf(key, sizeof key, "PC_CONV_%s", name);
	v = getenv(key);
	return (v != NULL && v[0] != '\0') ? v : NULL;
}

/* path below the PC_CONV_ROOT prefix (empty by default) into buf. */
static const char *cv_rooted(char *buf, size_t n, const char *path)
{
	const char *r = cv_env("ROOT");

	(void)snprintf(buf, n, "%s%s", r != NULL ? r : "", path);
	return buf;
}

/* /dev/<name>, or <PC_CONV_DEV>/<name>. */
static const char *cv_dev(char *buf, size_t n, const char *name)
{
	const char *d = cv_env("DEV");

	(void)snprintf(buf, n, "%s/%s", d != NULL ? d : "/dev", name);
	return buf;
}

/* The whole file in a malloc'ed, NUL-terminated buffer, or NULL. */
static char *cv_slurp(const char *path, size_t *lenp)
{
	FILE *f = fopen(path, "rb");
	size_t cap = 4096, len = 0, n;
	char *buf;

	if (f == NULL)
		return NULL;
	buf = malloc(cap);
	while (buf != NULL && (n = fread(buf + len, 1, cap - len - 1, f)) > 0) {
		len += n;
		if (cap - len < 2) {
			char *nb;

			if (cap > ((size_t)64 << 20))
				break;
			nb = realloc(buf, cap * 2);
			if (nb == NULL) {
				free(buf);
				buf = NULL;
				break;
			}
			buf = nb;
			cap *= 2;
		}
	}
	(void)fclose(f);
	if (buf == NULL)
		return NULL;
	buf[len] = '\0';
	if (lenp != NULL)
		*lenp = len;
	return buf;
}

/*
 * Next line of a buffer: returns a pointer to the NUL-terminated line (the
 * newline is overwritten) and advances *pp; NULL at the end.
 */
static char *cv_nextline(char **pp)
{
	char *s = *pp, *e;

	if (s == NULL || *s == '\0')
		return NULL;
	e = strchr(s, '\n');
	if (e != NULL) {
		*e = '\0';
		*pp = e + 1;
	} else {
		*pp = s + strlen(s);
	}
	return s;
}

/*
 * The text of an /etc database: PC_CONV_<envname> if set, else def. When the
 * file cannot be read the test is NOTINUSE (another database may be in use).
 */
static char *cv_etc(const char *envname, const char *def)
{
	const char *p = cv_env(envname);
	char *t = cv_slurp(p != NULL ? p : def, NULL);

	if (t == NULL)
		pc_notinuse("%s cannot be read: no such file database on this system", p != NULL ? p : def);
	return t;
}

/* Does every character of s satisfy the "decimal number" rule, fit 32 bits? */
static int cv_decimal32(const char *s, unsigned long *v)
{
	char *e;

	if (s[0] < '0' || s[0] > '9')
		return 0;
	*v = strtoul(s, &e, 10);
	return *e == '\0' && strlen(s) <= 10 && *v <= 4294967295UL;
}

/* Does path (followed through symbolic links) name a directory? */
static int cv_isdir(const char *path)
{
	struct stat st;

	return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

/* Split s in place on any of the characters in seps into at most max
 * fields (runs of separators collapse when collapse is non-zero). */
static int cv_split(char *s, const char *seps, int collapse, char **out, int max)
{
	int n = 0;

	for (;;) {
		if (collapse)
			s += strspn(s, seps);
		if (*s == '\0' && collapse)
			break;
		if (n == max)
			return -1;
		out[n++] = s;
		s += strcspn(s, seps);
		if (*s == '\0')
			break;
		*s++ = '\0';
	}
	return n;
}

/* Strip a '#' comment and trailing blanks in place. */
static void cv_strip_comment(char *s)
{
	char *h = strchr(s, '#');
	size_t n;

	if (h != NULL)
		*h = '\0';
	n = strlen(s);
	while (n > 0 && (s[n - 1] == ' ' || s[n - 1] == '\t' || s[n - 1] == '\r'))
		s[--n] = '\0';
}

/*
 * Does the name-service configuration let the file database answer first?
 * True when there is no configuration file, no line for db, or the first
 * source on the line is "files", "compat" or "db" (a map built from the file) (PC_CONV_NSSWITCH overrides the
 * configuration path). Used to decide whether a library lookup may be
 * compared with the file.
 */
static int cv_nss_files_first(const char *db)
{
	const char *cp = cv_env("NSSWITCH");
	char *t = cv_slurp(cp != NULL ? cp : "/etc/nsswitch.conf", NULL), *p, *l, *w;
	size_t n = strlen(db);
	int r = 1;

	if (t == NULL)
		return 1;
	p = t;
	while ((l = cv_nextline(&p)) != NULL) {
		l += strspn(l, " \t");
		if (strncmp(l, db, n) != 0 || l[n] != ':')
			continue;
		w = l + n + 1;
		w += strspn(w, " \t");
		if (*w != '\0' && *w != '#')
			r = strncmp(w, "files", 5) == 0 || strncmp(w, "compat", 6) == 0 ||
			    (strncmp(w, "db", 2) == 0 && (w[2] == ' ' || w[2] == '\t' || w[2] == '\0'));
	}
	free(t);
	return r;
}

#endif
