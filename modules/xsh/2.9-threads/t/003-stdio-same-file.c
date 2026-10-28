/*
 * Several threads writing lines to one stream with fputs() at once leave
 * every line intact.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.003
 *
 * Four threads each call fputs() 500 times on one FILE with a line of 40
 * letters (all the same letter, one letter per thread) ending in a newline.
 * Afterwards the file has 2000 lines, each uniform and complete, and 500
 * lines of each letter.
 */
#include "pcth.h"

#define NT 4
#define LINES 500
#define LEN 40

static FILE *fp;

static void *worker(void *a)
{
	char line[LEN + 2];
	int id = *(int *)a, i;

	memset(line, 'a' + id, LEN);
	line[LEN] = '\n';
	line[LEN + 1] = '\0';
	while (!flag_get(0))
		sched_yield();
	for (i = 0; i < LINES; i++)
		if (fputs(line, fp) == EOF)
			flag_add(1, 1);
	return NULL;
}

int main(void)
{
	pthread_t t[NT];
	int ids[NT], i, count[NT] = { 0 };
	char line[LEN + 8];
	FILE *in;

	pc_begin("XSH.2.9-threads.003");
	guard(120);
	fp = fopen("out", "w");
	NEED(fp != NULL, "fopen failed");
	pc_test();
	for (i = 0; i < NT; i++) {
		ids[i] = i;
		t[i] = start(worker, &ids[i]);
	}
	sleep_ms(50);
	flag_set(0, 1);
	for (i = 0; i < NT; i++)
		NEED(pthread_join(t[i], NULL) == 0, "pthread_join failed");
	NEED(flag_get(1) == 0, "fputs() failed");
	NEED(fclose(fp) == 0, "fclose failed");
	in = fopen("out", "r");
	NEED(in != NULL, "fopen of the result failed");
	while (fgets(line, sizeof line, in) != NULL) {
		int k;

		pc_check(strlen(line) == LEN + 1 && line[LEN] == '\n', "a line is not 40 letters and a newline (an fputs() call was split)");
		pc_check(line[0] >= 'a' && line[0] < 'a' + NT, "a line starts with an unexpected character");
		for (k = 1; k < LEN; k++)
			pc_check(line[k] == line[0], "a line mixes the output of two fputs() calls");
		count[line[0] - 'a']++;
	}
	(void)fclose(in);
	for (i = 0; i < NT; i++)
		pc_check(count[i] == LINES, "thread %d has %d lines in the file, expected %d", i, count[i], LINES);
	pc_pass();
}
