/*
 * sig2str-impl.inc - a sig2str() and str2sig() for the header variants
 * sig2str.rtmax_upper.h and sig2str.rtmin_all.h (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * The reference C libraries have neither function, so these variants
 * supply a whole conforming pair, defined static in the header that is
 * included before the test (the names are macros, so a later C library
 * that declares the functions does not clash). Which signal numbers are
 * valid follows kill() on the system: 1 to SIGRTMAX.
 *
 * Before including this file, define PCV_RTMAX_UPPER (1: names above the
 * midpoint of the realtime range are "RTMAX-m", and str2sig() also takes
 * names with the SIG prefix; 0: "RTMIN+n" throughout and nothing extra)
 * and PCV_SIG2STR_MAX.
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#undef SIG2STR_MAX
#define SIG2STR_MAX PCV_SIG2STR_MAX
#define sig2str pcv_sig2str
#define str2sig pcv_str2sig

static const struct { int n; const char *s; } pcv_table[] = {
	{ SIGABRT, "ABRT" }, { SIGALRM, "ALRM" }, { SIGBUS, "BUS" }, { SIGCHLD, "CHLD" },
	{ SIGCONT, "CONT" }, { SIGFPE, "FPE" }, { SIGHUP, "HUP" }, { SIGILL, "ILL" },
	{ SIGINT, "INT" }, { SIGKILL, "KILL" }, { SIGPIPE, "PIPE" }, { SIGQUIT, "QUIT" },
	{ SIGSEGV, "SEGV" }, { SIGSTOP, "STOP" }, { SIGTERM, "TERM" }, { SIGTSTP, "TSTP" },
	{ SIGTTIN, "TTIN" }, { SIGTTOU, "TTOU" }, { SIGUSR1, "USR1" }, { SIGUSR2, "USR2" },
	{ SIGWINCH, "WINCH" }, { SIGSYS, "SYS" }, { SIGTRAP, "TRAP" }, { SIGURG, "URG" },
	{ SIGVTALRM, "VTALRM" }, { SIGXCPU, "XCPU" }, { SIGXFSZ, "XFSZ" },
};

static inline int pcv_valid(int s)
{
	return s >= 1 && s <= SIGRTMAX;
}

/* A decimal number of 1 to 9 digits, or -1. */
static inline int pcv_decimal(const char *s)
{
	int n = 0, k = 0;

	for (; *s != '\0'; s++, k++) {
		if (*s < '0' || *s > '9' || k == 9)
			return -1;
		n = n * 10 + (*s - '0');
	}
	return k == 0 ? -1 : n;
}

/*
 * sig2str() DESCRIPTION: table signals by name without SIG, SIGRTMIN and
 * SIGRTMAX as RTMIN and RTMAX, RTMIN+n up to the midpoint, RTMIN+n or
 * RTMAX-m above it (PCV_RTMAX_UPPER picks), and any other valid signal in
 * an unspecified but unique form: here its decimal number, which
 * str2sig() takes back.
 */
static inline int pcv_base_sig2str(int s, char *str)
{
	size_t i;

	if (!pcv_valid(s))
		return -1;
	for (i = 0; i < sizeof pcv_table / sizeof pcv_table[0]; i++)
		if (pcv_table[i].n == s) {
			strcpy(str, pcv_table[i].s);
			return 0;
		}
	if (s == SIGRTMIN)
		strcpy(str, "RTMIN");
	else if (s == SIGRTMAX)
		strcpy(str, "RTMAX");
	else if (s > SIGRTMIN && s < SIGRTMAX && PCV_RTMAX_UPPER && s > (SIGRTMIN + SIGRTMAX) / 2)
		(void)sprintf(str, "RTMAX-%d", SIGRTMAX - s);
	else if (s > SIGRTMIN && s < SIGRTMAX)
		(void)sprintf(str, "RTMIN+%d", s - SIGRTMIN);
	else
		(void)sprintf(str, "%d", s);
	return 0;
}

/*
 * str2sig() DESCRIPTION: the table names, RTMIN, RTMAX, RTMIN+n and
 * RTMAX-n for n from 1 to SIGRTMAX-SIGRTMIN-1, decimal numbers of valid
 * signals; with PCV_RTMAX_UPPER also any of those names with SIG in front
 * (an additional implementation-dependent form). Anything else: -1, with
 * *pnum untouched.
 */
static inline int pcv_base_str2sig(const char *str, int *pnum)
{
	size_t i;
	int n, span = SIGRTMAX - SIGRTMIN;

	for (i = 0; i < sizeof pcv_table / sizeof pcv_table[0]; i++)
		if (strcmp(str, pcv_table[i].s) == 0) {
			*pnum = pcv_table[i].n;
			return 0;
		}
	if (strcmp(str, "RTMIN") == 0) {
		*pnum = SIGRTMIN;
		return 0;
	}
	if (strcmp(str, "RTMAX") == 0) {
		*pnum = SIGRTMAX;
		return 0;
	}
	if (strncmp(str, "RTMIN+", 6) == 0 && (n = pcv_decimal(str + 6)) >= 1 && n <= span - 1) {
		*pnum = SIGRTMIN + n;
		return 0;
	}
	if (strncmp(str, "RTMAX-", 6) == 0 && (n = pcv_decimal(str + 6)) >= 1 && n <= span - 1) {
		*pnum = SIGRTMAX - n;
		return 0;
	}
	if ((n = pcv_decimal(str)) != -1 && pcv_valid(n)) {
		*pnum = n;
		return 0;
	}
	if (PCV_RTMAX_UPPER && strncmp(str, "SIG", 3) == 0 && pcv_decimal(str + 3) == -1)
		return pcv_base_str2sig(str + 3, pnum);
	return -1;
}

/*
 * Fault hooks (CI only): the faults of mutants/sig2str.c, applied to this
 * implementation while PC_PHASE is "test" and PC_MUTANT names one, so the
 * tests can be shown to catch them under each header variant. The driver
 * does not combine --variant with --mutant; these are run by hand (build
 * the test with the header, set PC_MUTANT).
 */
static inline const char *pcv_fault(void)
{
	const char *ph = getenv("PC_PHASE"), *m = getenv("PC_MUTANT");

	if (ph == NULL || strcmp(ph, "test") != 0 || m == NULL || strncmp(m, "sig2str.", 8) != 0)
		return "";
	return m + 8;
}

static inline int pcv_sig2str(int s, char *str)
{
	const char *m = pcv_fault();
	int r;

	if (strcmp(m, "accepts_invalid") == 0 && (s < 0 || s > 64)) {
		strcpy(str, "BOGUS");
		return 0;
	}
	if (strcmp(m, "rt_off_by_one") == 0 && s > SIGRTMIN && s < SIGRTMAX)
		s++;
	r = pcv_base_sig2str(s, str);
	if (r == 0 && strcmp(m, "keeps_prefix") == 0) {
		char t[64];
		(void)snprintf(t, sizeof t, "SIG%s", str);
		strcpy(str, t);
	}
	if (r == 0 && strcmp(m, "rt_padded") == 0 && strncmp(str, "RTMIN+", 6) == 0 && str[7] == '\0') {
		str[8] = '\0';
		str[7] = str[6];
		str[6] = '0';
	}
	if (r == 0 && strcmp(m, "returns_one") == 0)
		return 1;
	return r;
}

static inline int pcv_str2sig(const char *str, int *pnum)
{
	const char *m = pcv_fault();
	int r;

	if (strcmp(m, "str2sig_wants_prefix") == 0 && strncmp(str, "SIG", 3) != 0)
		return -1;
	if (strcmp(m, "str2sig_no_numbers") == 0 && str[0] >= '0' && str[0] <= '9')
		return -1;
	r = pcv_base_str2sig(str, pnum);
	if (r == 0 && strcmp(m, "str2sig_rt_off_by_one") == 0 && (strncmp(str, "RTMIN+", 6) == 0 || strncmp(str, "RTMAX-", 6) == 0))
		(*pnum)++;
	if (r == -1 && strcmp(m, "str2sig_stores_on_failure") == 0)
		*pnum = 0;
	return r;
}
