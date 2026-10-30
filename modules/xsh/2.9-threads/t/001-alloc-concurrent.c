/*
 * Calls that allocate a shared resource, made by several threads at
 * once, never hand the same resource to two callers.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.001
 *
 * Four threads, released together, each take 200 blocks with malloc() and
 * fill them with a value of their own, and open and dup 25 descriptors
 * each. When all have finished, every block must still hold its owner's
 * value and no two blocks overlap, and all 200 descriptors are different.
 */
#include "pcth.h"
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/stat.h>

#define NT 4
#define NB 200
#define NF 25
#define BLK 64

struct res {
	int id;
	int bad;
	int err;
	unsigned char *blk[NB];
	int fds[2 * NF];
};

static struct res res[NT];

static void *worker(void *a)
{
	struct res *r = a;
	int i;

	while (!flag_get(0))
		sched_yield();
	for (i = 0; i < NB; i++) {
		unsigned char *p = malloc(BLK);

		if (p == NULL) {
			r->bad = 1;
			return NULL;
		}
		memset(p, r->id + 1, BLK);
		r->blk[i] = p;
	}
	for (i = 0; i < NF; i++) {
		int fd = open("f", O_RDONLY), d;

		r->fds[2 * i] = fd;
		if (fd == -1) {
			r->err = errno;
			r->bad = 2;
			return NULL;
		}
		d = dup(fd);
		r->fds[2 * i + 1] = d;
		if (d == -1) {
			r->err = errno;
			r->bad = 2;
			return NULL;
		}
	}
	return NULL;
}

int main(void)
{
	pthread_t t[NT];
	int i, j, k;

	pc_begin("XSH.2.9-threads.001");
	guard(120);
	pc_make_file("f", "x");
	/* The test holds 200 descriptors at once: raise the soft limit if it is lower (setup only). */
	{
		struct rlimit rl;

		if (getrlimit(RLIMIT_NOFILE, &rl) == 0 && rl.rlim_cur != RLIM_INFINITY && rl.rlim_cur < 512) {
			rl.rlim_cur = (rl.rlim_max == RLIM_INFINITY || rl.rlim_max >= 512) ? 512 : rl.rlim_max;
			(void)setrlimit(RLIMIT_NOFILE, &rl);
		}
	}
	pc_test();
	for (i = 0; i < NT; i++) {
		res[i].id = i;
		t[i] = start(worker, &res[i]);
	}
	sleep_ms(50);
	flag_set(0, 1);
	for (i = 0; i < NT; i++)
		NEED(pthread_join(t[i], NULL) == 0, "pthread_join failed");
	for (i = 0; i < NT; i++) {
		NEED(res[i].bad != 1, "malloc() failed");
		NEED(res[i].bad != 2, "open() or dup() failed: %s (the process may be limited in open descriptors)", pc_errname(res[i].err));
	}
	for (i = 0; i < NT; i++)
		for (j = 0; j < NB; j++)
			for (k = 0; k < BLK; k++)
				pc_check(res[i].blk[j][k] == (unsigned char)(i + 1),
				    "a block from malloc() was overwritten by another thread (thread %d, block %d)", i, j);
	for (i = 0; i < NT * NB; i++)
		for (j = i + 1; j < NT * NB; j++) {
			unsigned char *a = res[i / NB].blk[i % NB], *b = res[j / NB].blk[j % NB];

			pc_check(a + BLK <= b || b + BLK <= a, "two blocks from malloc() overlap");
		}
	for (i = 0; i < NT * 2 * NF; i++)
		for (j = i + 1; j < NT * 2 * NF; j++)
			pc_check(res[i / (2 * NF)].fds[i % (2 * NF)] != res[j / (2 * NF)].fds[j % (2 * NF)],
			    "two threads were given the same descriptor number %d", res[i / (2 * NF)].fds[i % (2 * NF)]);
	pc_pass();
}
