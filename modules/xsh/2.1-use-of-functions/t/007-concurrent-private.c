/*
 * Calls on data of their own, made by several threads at once, give the
 * results one thread gets alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.1-use-of-functions.007
 */
#include "pc.h"
#include <errno.h>
#include <fnmatch.h>
#include <pthread.h>
#include <regex.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>
#include <ctype.h>

#define NTHREADS 4
#define NVAR 64
#define ITER 40000

static char exp_fmt[NTHREADS][NVAR][64];
static char exp_time[NTHREADS][NVAR][40];
static char exp_err[64];
static pthread_barrier_t gate;

struct res {
	int bad;
	char msg[120];
};
static struct res result[NTHREADS];

static int cmpint(const void *a, const void *b)
{
	int x = *(const int *)a, y = *(const int *)b;

	return (x > y) - (x < y);
}

static void note(struct res *r, const char *what, int iter)
{
	if (!r->bad) {
		r->bad = 1;
		snprintf(r->msg, sizeof r->msg, "%s (iteration %d)", what, iter);
	}
}

static void *worker(void *arg)
{
	int t = (int)(intptr_t)arg, i, k;
	struct res *r = &result[t];
	regex_t re;
	int have_re = (regcomp(&re, "^a[0-9]+b$", REG_EXTENDED | REG_NOSUB) == 0);
	mbstate_t st;

	memset(&st, 0, sizeof st);
	(void)pthread_barrier_wait(&gate);
	for (i = 0; i < ITER && !r->bad; i++) {
		int v = i % NVAR;
		char buf[80], tbuf[48], num[24];
		int a[24], key, *hit;
		struct tm tm;
		time_t tt;
		char *end;
		long l;
		double d;
		wchar_t wc;
		char src[16], dst[16];

		snprintf(buf, sizeof buf, "%d:%s:%x:%.3f", v * 7 + t, "ab", v, v / 8.0);
		if (strcmp(buf, exp_fmt[t][v]) != 0)
			note(r, "snprintf gave a wrong string", i);
		snprintf(num, sizeof num, "%d", -(i * 3 + t));
		l = strtol(num, &end, 10);
		if (l != -(long)(i * 3 + t) || *end != '\0')
			note(r, "strtol gave a wrong value", i);
		d = strtod("3.25e2", &end);
		if (d != 325.0 || *end != '\0')
			note(r, "strtod gave a wrong value", i);
		for (k = 0; k < 24; k++)
			a[k] = (k * 7919 + t * 13 + i) % 101 * 2 + (k == 5);
		qsort(a, 24, sizeof a[0], cmpint);
		for (k = 1; k < 24; k++)
			if (a[k - 1] > a[k])
				note(r, "qsort left the array unsorted", i);
		key = a[i % 24];
		hit = bsearch(&key, a, 24, sizeof a[0], cmpint);
		if (hit == NULL || *hit != key)
			note(r, "bsearch did not find a member", i);
		memset(src, 'a' + t, sizeof src - 1);
		src[sizeof src - 1] = '\0';
		memcpy(dst, src, sizeof src);
		if (memcmp(dst, src, sizeof src) != 0 || strlen(dst) != sizeof dst - 1)
			note(r, "memcpy, memcmp or strlen gave a wrong result", i);
		tt = (time_t)(1700000000 + t * 100000 + v * 3600);
		if (gmtime_r(&tt, &tm) == NULL ||
		    strftime(tbuf, sizeof tbuf, "%Y-%m-%d %H:%M:%S", &tm) == 0 ||
		    strcmp(tbuf, exp_time[t][v]) != 0)
			note(r, "gmtime_r or strftime gave a wrong result", i);
		if (toupper('a' + (v % 26)) != 'A' + (v % 26))
			note(r, "toupper gave a wrong result", i);
		if (have_re) {
			snprintf(buf, sizeof buf, "a%db", i);
			if (regexec(&re, buf, 0, NULL, 0) != 0)
				note(r, "regexec did not match", i);
			snprintf(buf, sizeof buf, "a%dc", i);
			if (regexec(&re, buf, 0, NULL, 0) != REG_NOMATCH)
				note(r, "regexec matched wrongly", i);
		}
		snprintf(buf, sizeof buf, "file%d.c", i);
		if (fnmatch("*.c", buf, 0) != 0 || fnmatch("*.h", buf, 0) != FNM_NOMATCH)
			note(r, "fnmatch gave a wrong result", i);
		if (mbrtowc(&wc, "Q", 1, &st) != 1 || wc != L'Q')
			note(r, "mbrtowc gave a wrong result", i);
		if (strerror_r(EINVAL, buf, sizeof buf) != 0 || strcmp(buf, exp_err) != 0)
			note(r, "strerror_r gave a wrong result", i);
	}
	if (have_re)
		regfree(&re);
	return NULL;
}

static void on_alarm(int s)
{
	(void)s;
	pc_fail("the threads did not finish in time");
}

int main(void)
{
	pthread_t th[NTHREADS];
	int t, v, i, started = 0;

	pc_begin("XSH.2.1-use-of-functions.007");
	/* The expected results are made by one thread, before the others exist. */
	for (t = 0; t < NTHREADS; t++) {
		for (v = 0; v < NVAR; v++) {
			struct tm tm;
			time_t tt = (time_t)(1700000000 + t * 100000 + v * 3600);

			snprintf(exp_fmt[t][v], sizeof exp_fmt[t][v], "%d:%s:%x:%.3f", v * 7 + t, "ab", v, v / 8.0);
			if (gmtime_r(&tt, &tm) == NULL ||
			    strftime(exp_time[t][v], sizeof exp_time[t][v], "%Y-%m-%d %H:%M:%S", &tm) == 0)
				pc_unresolved("cannot make the expected time strings");
		}
	}
	if (strerror_r(EINVAL, exp_err, sizeof exp_err) != 0 || exp_err[0] == '\0')
		pc_unresolved("strerror_r(EINVAL) failed");
	if (pthread_barrier_init(&gate, NULL, NTHREADS) != 0)
		pc_unresolved("pthread_barrier_init failed");
	(void)signal(SIGALRM, on_alarm);
	pc_test();
	(void)alarm(50);
	for (t = 0; t < NTHREADS; t++) {
		if (pthread_create(&th[t], NULL, worker, (void *)(intptr_t)t) != 0)
			pc_unresolved("pthread_create failed");
		started++;
	}
	for (t = 0; t < started; t++)
		(void)pthread_join(th[t], NULL);
	for (i = 0; i < NTHREADS; i++)
		pc_check(!result[i].bad, "thread %d: %s", i, result[i].msg);
	pc_pass();
}
