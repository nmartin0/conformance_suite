/*
 * Functions that keep no state between calls give each of several
 * concurrent threads the results a single thread gets.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.002
 *
 * Four threads, released together, repeat 300 times: strerror_r() of an
 * error number of their own, gmtime_r() of a time of their own, rand_r()
 * from a seed of their own, strtok_r() over a string of their own,
 * snprintf() of a number of their own. Each result is compared with the
 * one the main thread got before any thread started.
 */
#include "pcth.h"

#include <stdint.h>

#define NT 4
#define LOOPS 300

struct job {
	int id;
	int bad;
	int err;
	time_t when;
	unsigned seed;
	char want_msg[128];
	struct tm want_tm;
	int want_rand[4];
	char text[64];
	char want_tok[3][16];
	char want_num[32];
};

static struct job jobs[NT];

static int same_tm(const struct tm *a, const struct tm *b)
{
	return a->tm_sec == b->tm_sec && a->tm_min == b->tm_min && a->tm_hour == b->tm_hour &&
	    a->tm_mday == b->tm_mday && a->tm_mon == b->tm_mon && a->tm_year == b->tm_year &&
	    a->tm_wday == b->tm_wday && a->tm_yday == b->tm_yday && a->tm_isdst == b->tm_isdst;
}
static const int errs[NT] = { ENOENT, EACCES, EEXIST, EINVAL };

static void *worker(void *a)
{
	struct job *j = a;
	int n;

	while (!flag_get(0))
		sched_yield();
	for (n = 0; n < LOOPS && !j->bad; n++) {
		char msg[128], num[32], buf[64], *save, *tok;
		struct tm tm;
		unsigned s = j->seed;
		int k;

		if (strerror_r(j->err, msg, sizeof msg) != 0 || strcmp(msg, j->want_msg) != 0)
			j->bad = 1;
		if (gmtime_r(&j->when, &tm) == NULL || !same_tm(&tm, &j->want_tm))
			j->bad = 2;
		for (k = 0; k < 4; k++)
			if (rand_r(&s) != j->want_rand[k])
				j->bad = 3;
		strcpy(buf, j->text);
		tok = strtok_r(buf, ",", &save);
		for (k = 0; k < 3; k++) {
			if (tok == NULL || strcmp(tok, j->want_tok[k]) != 0) {
				j->bad = 4;
				break;
			}
			tok = strtok_r(NULL, ",", &save);
		}
		if (tok != NULL)
			j->bad = 4;
		(void)snprintf(num, sizeof num, "%d:%05d:%x", j->id, 1000 * j->id + 7, 0xabc0 + (unsigned)j->id);
		if (strcmp(num, j->want_num) != 0)
			j->bad = 5;
	}
	return NULL;
}

int main(void)
{
	pthread_t t[NT];
	int i, k;

	pc_begin("XSH.2.9-threads.002");
	guard(120);
	for (i = 0; i < NT; i++) {
		struct job *j = &jobs[i];
		unsigned s;
		char buf[64], *save, *tok;

		j->id = i;
		j->err = errs[i];
		j->when = (time_t)(86400L * (365L * (i + 1) + 17) + 3600L * i + 61L * i);
		j->seed = 1000u + 77u * (unsigned)i;
		NEED(strerror_r(j->err, j->want_msg, sizeof j->want_msg) == 0, "strerror_r failed");
		NEED(gmtime_r(&j->when, &j->want_tm) != NULL, "gmtime_r failed");
		s = j->seed;
		for (k = 0; k < 4; k++)
			j->want_rand[k] = rand_r(&s);
		(void)snprintf(j->text, sizeof j->text, "a%d,bb%d,ccc%d", i, i, i);
		strcpy(buf, j->text);
		tok = strtok_r(buf, ",", &save);
		for (k = 0; k < 3; k++) {
			NEED(tok != NULL, "strtok_r gave too few tokens");
			strcpy(j->want_tok[k], tok);
			tok = strtok_r(NULL, ",", &save);
		}
		(void)snprintf(j->want_num, sizeof j->want_num, "%d:%05d:%x", i, 1000 * i + 7, 0xabc0 + (unsigned)i);
	}
	pc_test();
	for (i = 0; i < NT; i++)
		t[i] = start(worker, &jobs[i]);
	sleep_ms(50);
	flag_set(0, 1);
	for (i = 0; i < NT; i++)
		NEED(pthread_join(t[i], NULL) == 0, "pthread_join failed");
	for (i = 0; i < NT; i++)
		pc_check(jobs[i].bad == 0, "thread %d got a different result than the main thread from %s", i,
		    jobs[i].bad == 1 ? "strerror_r()" : jobs[i].bad == 2 ? "gmtime_r()" : jobs[i].bad == 3 ? "rand_r()" :
		    jobs[i].bad == 4 ? "strtok_r()" : "snprintf()");
	pc_pass();
}
