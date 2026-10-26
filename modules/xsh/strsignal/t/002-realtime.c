/*
 * strsignal() gives psignal()'s description for realtime signals.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strsignal.002
 * MUTANT    strsignal.null_rt
 * MUTANT    strsignal.text_differs
 * VARIANT   strsignal.own_text
 * VARIANT   strsignal.static_buf
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* What psignal(sig, "pc") writes to standard error. */
static char out[1024];

static const char *psignal_text(int sig)
{
	int p[2], saved;
	ssize_t k, n = 0;

	NEED(pipe(p) == 0 && (saved = dup(2)) != -1, "pipe or dup failed");
	(void)fflush(stderr);
	NEED(dup2(p[1], 2) == 2, "dup2 failed");
	psignal(sig, "pc");
	(void)fflush(stderr);
	(void)dup2(saved, 2);
	(void)close(saved);
	(void)close(p[1]);
	while (n < (ssize_t)sizeof out - 1 && (k = read(p[0], out + n, sizeof out - 1 - (size_t)n)) > 0)
		n += k;
	(void)close(p[0]);
	out[n] = '\0';
	return out;
}

/* FAIL unless strsignal(sig) is psignal()'s description of sig. */
static void same(int sig, const char *name)
{
	char want[1024];
	const char *s = strsignal(sig);

	pc_check(s != NULL, "strsignal(%s) returned a null pointer", name);
	if (strlen(s) + 6 >= sizeof want)
		pc_unresolved("strsignal(%s) is too long for the test", name);
	(void)snprintf(want, sizeof want, "pc: %s\n", s);	/* copied before psignal() runs */
	(void)psignal_text(sig);
	pc_check(strcmp(out, want) == 0, "for %s psignal() wrote \"%s\" but strsignal() gave \"%.*s\"",
	    name, out, (int)strlen(want) - 5, want + 4);
}

int main(void)
{
	pc_begin("XSH.strsignal.002");
	pc_check(SIGRTMIN + 1 <= SIGRTMAX, "SIGRTMIN+1 is above SIGRTMAX");
	pc_test();
	same(SIGRTMIN, "SIGRTMIN");
	same(SIGRTMIN + 1, "SIGRTMIN+1");
	same(SIGRTMAX, "SIGRTMAX");
	pc_pass();
}
