/*
 * fstat() made while another thread calls ftruncate() or futimens() on the
 * same file sees all or none of the change.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.032
 *
 * One thread truncates a file alternately to 0 and to 1 MiB; another sets
 * the access and modification times of a second file to the same whole
 * second (a new one each time) with futimens(); for 1.5 s two threads call
 * fstat() on them. The size seen is always 0 or 1 MiB, and the two times
 * seen are always equal.
 */
#include "pcth.h"
#include <fcntl.h>
#include <sys/stat.h>

#define BIG (1L << 20)

static int fa, fb;

static void *trunc_thread(void *a)
{
	int n = 0;

	(void)a;
	while (!flag_get(1))
		if (ftruncate(fa, (n++ & 1) ? 0 : BIG) != 0)
			flag_add(2, 1);
	return NULL;
}

static void *times_thread(void *a)
{
	long n = 5000;

	(void)a;
	while (!flag_get(1)) {
		struct timespec ts[2];

		ts[0].tv_sec = ts[1].tv_sec = n++;
		ts[0].tv_nsec = ts[1].tv_nsec = 0;
		if (futimens(fb, ts) != 0)
			flag_add(2, 1);
	}
	return NULL;
}

static void *stat_thread(void *a)
{
	struct stat s;

	(void)a;
	while (!flag_get(1)) {
		if (fstat(fa, &s) != 0)
			flag_add(2, 1);
		else if (s.st_size != 0 && s.st_size != BIG)
			flag_add(3, 1);
		if (fstat(fb, &s) != 0)
			flag_add(2, 1);
		else if (s.st_atim.tv_sec != s.st_mtim.tv_sec || s.st_atim.tv_nsec != s.st_mtim.tv_nsec)
			flag_add(4, 1);
		flag_add(5, 1);
	}
	return NULL;
}

int main(void)
{
	pthread_t a, b, s1, s2;
	struct timespec ts[2];

	pc_begin("XSH.2.9-threads.032");
	guard(120);
	fa = open("sz", O_RDWR | O_CREAT | O_TRUNC, 0600);
	fb = open("tm", O_RDWR | O_CREAT | O_TRUNC, 0600);
	NEED(fa != -1 && fb != -1, "open failed");
	ts[0].tv_sec = ts[1].tv_sec = 4000;
	ts[0].tv_nsec = ts[1].tv_nsec = 0;
	NEED(futimens(fb, ts) == 0, "futimens failed");
	pc_test();
	s1 = start(stat_thread, NULL);
	s2 = start(stat_thread, NULL);
	a = start(trunc_thread, NULL);
	b = start(times_thread, NULL);
	sleep_ms(1500);
	flag_set(1, 1);
	NEED(pthread_join(a, NULL) == 0 && pthread_join(b, NULL) == 0 && pthread_join(s1, NULL) == 0 && pthread_join(s2, NULL) == 0,
	    "pthread_join failed");
	NEED(flag_get(2) == 0, "a call failed");
	NEED(flag_get(5) >= 100, "fewer than 100 fstat() rounds were made");
	pc_check(flag_get(3) == 0, "fstat() saw a size that is neither 0 nor 1 MiB");
	pc_check(flag_get(4) == 0, "fstat() saw access and modification times that futimens() never set together");
	pc_pass();
}
