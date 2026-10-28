/*
 * read() calls made by several threads on one descriptor are atomic with
 * respect to each other, including the use of the shared file offset.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.030
 *
 * A file holds 60000 records of four bytes, each carrying its own index.
 * Four threads read it to the end, four bytes at a time, from one
 * descriptor. Every record must be read exactly once, whole.
 */
#include "pcth.h"
#include <fcntl.h>

#define NT 4
#define RECS 60000

static int fd;
static unsigned char seen[RECS];
static int count[NT];

static void *worker(void *a)
{
	unsigned char b[4];
	int id = *(int *)a;
	ssize_t r;

	while (!flag_get(0))
		sched_yield();
	for (;;) {
		r = read(fd, b, 4);
		if (r == 0)
			break;
		if (r != 4 || b[3] != 0xA5) {
			flag_add(1, 1);
			break;
		}
		{
			unsigned idx = (unsigned)b[0] | ((unsigned)b[1] << 8) | ((unsigned)b[2] << 16);

			if (idx >= RECS)
				flag_add(1, 1);
			else
				seen[idx]++;	/* a second reader of the same record would show as 2 */
		}
		count[id]++;
	}
	return NULL;
}

int main(void)
{
	pthread_t t[NT];
	int ids[NT], i, total = 0;
	unsigned char rec[4];
	FILE *f;

	pc_begin("XSH.2.9-threads.030");
	guard(120);
	f = fopen("recs", "wb");
	NEED(f != NULL, "fopen failed");
	for (i = 0; i < RECS; i++) {
		rec[0] = (unsigned char)(i & 255);
		rec[1] = (unsigned char)((i >> 8) & 255);
		rec[2] = (unsigned char)((i >> 16) & 255);
		rec[3] = 0xA5;
		NEED(fwrite(rec, 1, 4, f) == 4, "fwrite failed");
	}
	NEED(fclose(f) == 0, "fclose failed");
	fd = open("recs", O_RDONLY);
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
	pc_check(flag_get(1) == 0, "a read() returned a partial or damaged record");
	for (i = 0; i < NT; i++)
		total += count[i];
	pc_check(total == RECS, "the threads read %d records between them, expected %d", total, RECS);
	for (i = 0; i < RECS; i++)
		pc_check(seen[i] == 1, "record %d was read %d times", i, seen[i]);
	pc_pass();
}
