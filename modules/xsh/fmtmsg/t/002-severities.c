/*
 * fmtmsg() severity strings.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fmtmsg.002
 */
#include "pc.h"
#include <errno.h>
#include <fmtmsg.h>
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
 * Runs fmtmsg(MM_PRINT | cls_extra, ...) in a child with MSGVERB set to
 * msgverb (unset when NULL) and descriptor 2 on a pipe. Returns the
 * fmtmsg() result (as the child's exit status mapped back) and puts the
 * standard error output in out.
 */
static int run(const char *msgverb, long cls, const char *label, int severity, const char *text,
    const char *action, const char *tag, char *out, size_t outn)
{
	int p[2], st;
	pid_t c;

	if (pipe(p) != 0)
		pc_unresolved("pipe failed");
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (c == 0) {
		int r;

		(void)close(p[0]);
		if (dup2(p[1], 2) != 2)
			_exit(99);
		if (msgverb)
			(void)setenv("MSGVERB", msgverb, 1);
		else
			(void)unsetenv("MSGVERB");
		alarm(20);
		r = fmtmsg(cls, label, severity, text, action, tag);
		(void)fflush(stderr);
		_exit(r == MM_OK ? 0 : r == MM_NOTOK ? 1 : r == MM_NOMSG ? 2 : r == MM_NOCON ? 3 : 9);
	}
	(void)close(p[1]);
	(void)slurp(p[0], out, outn);
	(void)close(p[0]);
	while (waitpid(c, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	pc_check(WIFEXITED(st), "the child did not exit normally (status %#x)", st);
	return WEXITSTATUS(st);
}

#define PRINT (MM_PRINT | MM_SOFT | MM_APPL | MM_RECOVER)

/* Position of s in out, or NULL. */
static const char *at(const char *out, const char *s)
{
	return strstr(out, s);
}

int main(void)
{
	static const struct { int sev; const char *name; } sevs[] = {
		{ MM_HALT, "HALT" }, { MM_ERROR, "ERROR" }, { MM_WARNING, "WARNING" }, { MM_INFO, "INFO" },
	};
	static const char *const all[] = { "HALT", "ERROR", "WARNING", "INFO" };
	char out[2048];
	size_t i, j;
	int r;

	pc_begin("XSH.fmtmsg.002");
	guard(60);
	pc_test();
	for (i = 0; i < 4; i++) {
		r = run(NULL, PRINT, "XSI:sev", sevs[i].sev, "something odd", "retry", "XSI:sev:1", out, sizeof out);
		pc_check(r == 0, "fmtmsg() did not return MM_OK for %s (code %d)", sevs[i].name, r);
		pc_check(at(out, sevs[i].name) != NULL, "%s does not appear: \"%s\"", sevs[i].name, out);
		for (j = 0; j < 4; j++)
			if (j != i)
				pc_check(at(out, all[j]) == NULL, "%s appears for %s: \"%s\"", all[j], sevs[i].name, out);
	}
	r = run(NULL, PRINT, "XSI:sev", MM_NOSEV, "something odd", "retry", "XSI:sev:1", out, sizeof out);
	pc_check(r == 0, "fmtmsg() did not return MM_OK with MM_NOSEV (code %d)", r);
	for (j = 0; j < 4; j++)
		pc_check(at(out, all[j]) == NULL, "%s appears with MM_NOSEV: \"%s\"", all[j], out);
	pc_check(at(out, "something odd") != NULL, "the text is missing with MM_NOSEV: \"%s\"", out);
	pc_pass();
}
