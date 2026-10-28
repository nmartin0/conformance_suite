/*
 * write() calls made by several threads on one descriptor are atomic with
 * respect to each other, including the use of the shared file offset.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.029
 *
 * Four threads each write 1500 blocks of 4096 bytes, all of one letter
 * (a different one per thread), to one descriptor of a regular file
 * opened for writing. Afterwards the file is 6000 blocks long, every block
 * is uniform, and there are 1500 blocks of each letter.
 */
#include "pcth.h"
#include <fcntl.h>
#include <sys/stat.h>

#define NT 4
#define CH 4096
#define PER 1500

static int fd;

static void *worker(void *a)
{
	char buf[CH];
	int id = *(int *)a, i;

	memset(buf, 'a' + id, CH);
	while (!flag_get(0))
		sched_yield();
	for (i = 0; i < PER; i++)
		if (write(fd, buf, CH) != CH)
			flag_add(1, 1);
	return NULL;
}

int main(void)
{
	pthread_t t[NT];
	int ids[NT], i, cnt[NT] = { 0 }, in;
	struct stat st;
	char buf[CH];
	long k;

	pc_begin("XSH.2.9-threads.029");
	guard(120);
	fd = open("w", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	NEED(fd != -1, "open failed");
	pc_test();
	for (i = 0; i < NT; i++) {
		ids[i] = i;
		t[i] = start(worker, &ids[i]);
	}
	sleep_ms(50);
	flag_set(0, 1);
	for (i = 0; i < NT; i++)
		NEED(pthread_join(t[i], NULL) == 0, "pthread_join failed");
	NEED(flag_get(1) == 0, "a write() failed or was short");
	NEED(close(fd) == 0, "close failed");
	NEED(stat("w", &st) == 0, "stat failed");
	pc_check(st.st_size == (off_t)NT * PER * CH, "the file is %ld bytes long, expected %ld (blocks overwrote each other)",
	    (long)st.st_size, (long)NT * PER * CH);
	in = open("w", O_RDONLY);
	NEED(in != -1, "open of the result failed");
	for (k = 0; k < (long)NT * PER; k++) {
		int j;

		NEED(read(in, buf, CH) == CH, "read of the result failed");
		for (j = 1; j < CH; j++)
			pc_check(buf[j] == buf[0], "block %ld of the file mixes the output of two write() calls", k);
		pc_check(buf[0] >= 'a' && buf[0] < 'a' + NT, "block %ld holds an unexpected byte", k);
		cnt[buf[0] - 'a']++;
	}
	(void)close(in);
	for (i = 0; i < NT; i++)
		pc_check(cnt[i] == PER, "thread %d has %d blocks in the file, expected %d", i, cnt[i], PER);
	pc_pass();
}
