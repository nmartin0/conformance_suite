/*
 * getopt() option-arguments.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getopt.002
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static void on_guard(int sig)
{
	(void)sig;
	pc_fail("blocked: a child did not finish in time");
}

static void guard(unsigned secs)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_guard;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	alarm(secs);
}

static size_t slurp(int fd, char *buf, size_t n)
{
	size_t got = 0;
	ssize_t k;

	while (got < n - 1) {
		k = read(fd, buf + got, n - 1 - got);
		if (k > 0)
			got += (size_t)k;
		else if (k == 0)
			break;
		else if (errno != EINTR)
			pc_unresolved("read failed: %s", pc_errname(errno));
	}
	buf[got] = '\0';
	return got;
}

/*
 * Runs one getopt() scan in a child whose getopt state is the system's own
 * initial one. The result string lists each return value (with optarg or
 * optopt where they apply); after the first '?' or ':' it ends with "!",
 * otherwise with the final optind and the operands left. Everything the
 * child wrote to standard error goes to err.
 */
static void scan(const char *opts, int diag_on, int argc, char **argv, char *res, size_t rn, char *err, size_t en)
{
	int pr[2], pe[2], st;
	pid_t c;

	if (pipe(pr) != 0 || pipe(pe) != 0)
		pc_unresolved("pipe failed");
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (c == 0) {
		char out[512];
		size_t len = 0;
		int ch, stopped = 0, i;

		(void)close(pr[0]);
		(void)close(pe[0]);
		if (dup2(pe[1], 2) != 2)
			_exit(99);
		if (!diag_on)
			opterr = 0;
		while ((ch = getopt(argc, argv, opts)) != -1) {
			const char *p = strchr(opts, ch);

			if (ch == '?' || ch == ':') {
				len += (size_t)snprintf(out + len, sizeof out - len, "%c(%c) ", ch, optopt ? optopt : '0');
				stopped = 1;
				break;
			}
			if (p != NULL && p[1] == ':')
				len += (size_t)snprintf(out + len, sizeof out - len, "%c=%s ", ch, optarg ? optarg : "(null)");
			else
				len += (size_t)snprintf(out + len, sizeof out - len, "%c ", ch);
			if (len > sizeof out - 80)
				break;
		}
		if (stopped)
			len += (size_t)snprintf(out + len, sizeof out - len, "!");
		else {
			len += (size_t)snprintf(out + len, sizeof out - len, "|%d ", optind);
			len += (size_t)snprintf(out + len, sizeof out - len, "rest=");
			for (i = optind; i < argc; i++)
				len += (size_t)snprintf(out + len, sizeof out - len, "%s%s", i > optind ? "," : "", argv[i]);
		}
		{
			ssize_t w = write(pr[1], out, len);

			(void)w;
		}
		_exit(0);
	}
	(void)close(pr[1]);
	(void)close(pe[1]);
	(void)slurp(pr[0], res, rn);
	(void)slurp(pe[0], err, en);
	(void)close(pr[0]);
	(void)close(pe[0]);
	while (waitpid(c, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (!WIFEXITED(st) || WEXITSTATUS(st) != 0)
		pc_fail("the scanning child ended abnormally (status %#x)", st);
}

#define CASE(opts, expect, ...) do { \
	char *av[] = { (char *)"prog", __VA_ARGS__, NULL }; \
	int ac = (int)(sizeof av / sizeof av[0]) - 1; \
	char res[600], err[600]; \
	scan(opts, 1, ac, av, res, sizeof res, err, sizeof err); \
	pc_check(strcmp(res, expect) == 0, "optstring \"%s\", arguments \"%s...\": got \"%s\", expected \"%s\"", \
	    opts, ac > 1 ? av[1] : "", res, expect); \
} while (0)

int main(void)
{
	pc_begin("XSH.getopt.002");
	guard(60);
	pc_test();
	CASE("a:b", "a=foo |3 rest=", "-a", "foo");
	CASE("a:b", "a=foo |2 rest=", "-afoo");
	CASE("a:b", "b a=foo |2 rest=", "-bafoo");
	CASE("a:b", "b a=foo |4 rest=bar", "-b", "-a", "foo", "bar");
	CASE("a:b", "a=-b |3 rest=", "-a", "-b");
	CASE("a:b", "a=-- |3 rest=", "-a", "--");
	CASE("a:b", "a=-- |3 rest=x", "-a", "--", "x");
	CASE("a:b:", "a=1 b=2 |5 rest=", "-a", "1", "-b", "2");
	CASE("a:b:", "a=1 b=2 |3 rest=", "-a1", "-b2");
	pc_pass();
}
