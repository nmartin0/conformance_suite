/*
 * A semaphore of value 1 locks out other threads between sem_wait() and
 * sem_post(): four threads never overlap in the section and lose no update.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sem_wait.006
 */
#include "semh.h"

#define NT 4
#define ROUNDS 2000

static sem_t lock;
static atomic_int inside, violations;
static long counter;

static int worker(void *p)
{
	int i, v;

	(void)p;
	for (i = 0; i < ROUNDS; i++) {
		if (sem_wait(&lock) != 0)
			return 1;
		if (atomic_fetch_add(&inside, 1) != 0)
			atomic_fetch_add(&violations, 1);
		v = (int)counter;
		counter = v + 1;
		atomic_fetch_sub(&inside, 1);
		if (sem_post(&lock) != 0)
			return 2;
	}
	return 0;
}

int main(void)
{
	struct job j[NT];
	int i;

	pc_begin("XSH.sem_wait.006");
	guard(50);
	new_sem(&lock, 0, 1);
	pc_test();
	for (i = 0; i < NT; i++)
		job_start(&j[i], worker, NULL);
	for (i = 0; i < NT; i++) {
		job_finish(&j[i], "a worker thread");
		pc_check(j[i].result == 0, "a worker's sem_wait() or sem_post() failed (%d)", j[i].result);
	}
	pc_check(atomic_load(&violations) == 0, "%d times two threads were in the section at once",
	    atomic_load(&violations));
	pc_check(counter == (long)NT * ROUNDS, "the counter is %ld, expected %ld", counter, (long)NT * ROUNDS);
	pc_pass();
}
