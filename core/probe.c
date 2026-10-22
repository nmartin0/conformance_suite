/*
 * probe - report what this system claims, for posixcheck.sh.
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Compiled with -D_XOPEN_SOURCE=800 so that both POSIX and XSI option
 * macros are visible, exactly as the tests will see them.
 *
 * usage:
 *   probe claims     one line per option:
 *                    claim CODE MACRO yes|no macro=V sysconf=V
 *   probe sysinfo    uname fields, one "key value" per line
 *   probe cspath     the value of confstr(_CS_PATH)
 *   probe euid       the effective user ID
 *   probe now        current UTC time, ISO 8601
 *   probe sleep N    sleep N seconds (the driver has no trusted sleep)
 *
 * Interpretation follows XBD <unistd.h>: an option macro that is
 * undefined or -1 means not supported; greater than zero means always
 * supported; zero means "ask sysconf() at run time".
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>

struct option {
	const char *code, *macro;
	int defined;
	long value;
	int has_sc;
	int sc;
};

static const struct option options[] = {
#include "probe_options.h"
};

static int claims(void)
{
	size_t i;

	printf("claim BASE - yes base\n");
	for (i = 0; i < sizeof options / sizeof options[0]; i++) {
		const struct option *o = &options[i];
		char mv[32], sv[32];
		const char *yes;
		long s;

		if (!o->defined)
			(void)snprintf(mv, sizeof mv, "undefined");
		else
			(void)snprintf(mv, sizeof mv, "%ld", o->value);
		(void)snprintf(sv, sizeof sv, "-");

		if (!o->defined || o->value == -1) {
			yes = "no";
		} else if (o->value > 0) {
			yes = "yes";
		} else if (!o->has_sc) {
			yes = "no";
			(void)snprintf(sv, sizeof sv, "no-name");
		} else {
			s = sysconf(o->sc);
			(void)snprintf(sv, sizeof sv, "%ld", s);
			yes = s > 0 ? "yes" : "no";
		}
		printf("claim %s %s %s macro=%s sysconf=%s\n", o->code,
		    o->macro, yes, mv, sv);
	}
	return 0;
}

static int sysinfo(void)
{
	struct utsname u;
	if (uname(&u) < 0)
		return 1;
	printf("sysname %s\nnodename %s\nrelease %s\nversion %s\n"
	    "machine %s\n", u.sysname, u.nodename, u.release, u.version,
	    u.machine);
#ifdef _POSIX_VERSION
	printf("posix_version %ld\n", (long)_POSIX_VERSION);
#endif
#ifdef _XOPEN_VERSION
	printf("xopen_version %ld\n", (long)_XOPEN_VERSION);
#endif
	return 0;
}

static int cspath(void)
{
	char buf[4096];
	size_t n = confstr(_CS_PATH, buf, sizeof buf);
	if (n == 0 || n > sizeof buf)
		return 1;
	printf("%s\n", buf);
	return 0;
}

static int now(void)
{
	char buf[64];
	time_t t = time(NULL);
	struct tm *tm = gmtime(&t);
	if (tm == NULL || strftime(buf, sizeof buf, "%Y-%m-%dT%H:%M:%SZ",
	    tm) == 0)
		return 1;
	printf("%s\n", buf);
	return 0;
}

int main(int argc, char **argv)
{
	if (argc == 2 && strcmp(argv[1], "claims") == 0)
		return claims();
	if (argc == 2 && strcmp(argv[1], "sysinfo") == 0)
		return sysinfo();
	if (argc == 2 && strcmp(argv[1], "cspath") == 0)
		return cspath();
	if (argc == 2 && strcmp(argv[1], "euid") == 0)
		return printf("%ld\n", (long)geteuid()) < 0;
	if (argc == 2 && strcmp(argv[1], "now") == 0)
		return now();
	if (argc == 3 && strcmp(argv[1], "sleep") == 0) {
		unsigned left = (unsigned)strtoul(argv[2], NULL, 10);
		while (left > 0)
			left = sleep(left);
		return 0;
	}
	fprintf(stderr, "usage: probe claims|sysinfo|cspath|euid|now|"
	    "sleep N\n");
	return 2;
}
