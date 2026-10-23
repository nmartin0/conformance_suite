/*
 * Concurrent O_APPEND writers never overwrite each other.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.005
 * MUTANT    write.append_racy
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void)
{
	char *buf, rec[11];
	int i, k, st, fd, seen[2][500];
	pid_t pid[2];
	long size;

	pc_begin("XSH.write.005");
	pc_make_file("f", "");
	pc_require_space();
	pc_test();
	for (k = 0; k < 2; k++) {
		pid[k] = fork();
		if (pid[k] == -1)
			pc_unresolved("fork failed");
		if (pid[k] == 0) {
			int w = open("f", O_WRONLY | O_APPEND);
			if (w < 0)
				_exit(2);
			for (i = 0; i < 500; i++) {
				snprintf(rec, sizeof rec, "%c%08d\n", 'a' + k, i);
				if (write(w, rec, 10) != 10)
					_exit(1);
			}
			_exit(0);
		}
	}
	for (k = 0; k < 2; k++)
		if (waitpid(pid[k], &st, 0) != pid[k] || !WIFEXITED(st) || WEXITSTATUS(st) != 0)
			pc_unresolved("a writer failed");
	size = pc_file_size("f");
	pc_check(size == 10000, "the file has %ld bytes, expected 10000: appends were lost", size);
	buf = malloc(10000);
	fd = open("f", O_RDONLY);
	if (buf == NULL || fd < 0 || read(fd, buf, 10000) != 10000)
		pc_unresolved("cannot read the file back");
	memset(seen, 0, sizeof seen);
	for (i = 0; i < 1000; i++) {
		char *r = buf + i * 10;
		int who = r[0] - 'a', n = atoi(r + 1);
		pc_check((who == 0 || who == 1) && n >= 0 && n < 500 && r[9] == '\n',
		    "record %d is damaged", i);
		seen[who][n]++;
	}
	for (k = 0; k < 2; k++)
		for (i = 0; i < 500; i++)
			pc_check(seen[k][i] == 1, "record %c%d appears %d times", 'a' + k, i, seen[k][i]);
	pc_pass();
}
