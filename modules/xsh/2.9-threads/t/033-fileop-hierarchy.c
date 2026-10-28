/*
 * Calls that change the file hierarchy are atomic with respect to calls
 * that look at it, made by other threads.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.033
 *
 * Three rounds, each with a thread that changes names for 1 s and one that
 * looks at them as fast as it can:
 *   1. the first creates a new file and rename()s it over "cur": stat()
 *      and open() of "cur" always succeed and see a one byte file;
 *   2. the first does link() then unlink() of a second name of a file:
 *      stat() of the file always sees a link count of 1 or 2;
 *   3. the first symlink()s a 200 character target to a name and
 *      unlink()s it: readlink() and lstat() see either no such file or a
 *      complete link whose whole target is one of the two targets.
 */
#include "pcth.h"
#include <fcntl.h>
#include <sys/stat.h>

static int round_no;

static void *changer(void *a)
{
	char t0[201], t1[201];
	long end = now_ms() + 1000;
	int n = 0;

	(void)a;
	memset(t0, 'a', 200);
	t0[200] = '\0';
	memset(t1, 'b', 200);
	t1[200] = '\0';
	while (now_ms() < end) {
		if (round_no == 1) {
			int fd = open("new", O_WRONLY | O_CREAT | O_TRUNC, 0600);

			if (fd == -1 || write(fd, "x", 1) != 1 || close(fd) != 0 || rename("new", "cur") != 0)
				flag_add(2, 1);
		} else if (round_no == 2) {
			if (link("cur", "ln") != 0 || unlink("ln") != 0)
				flag_add(2, 1);
		} else {
			if (symlink((n++ & 1) ? t1 : t0, "sl") != 0 || unlink("sl") != 0)
				flag_add(2, 1);
		}
	}
	flag_set(1, 1);
	return NULL;
}

static void *looker(void *a)
{
	struct stat s;
	char buf[400];
	int fd, i;
	ssize_t r;

	(void)a;
	while (!flag_get(1)) {
		if (round_no == 1) {
			if (stat("cur", &s) != 0)
				flag_add(3, 1);
			else if (s.st_size != 1)
				flag_add(4, 1);
			fd = open("cur", O_RDONLY);
			if (fd == -1)
				flag_add(3, 1);
			else
				(void)close(fd);
			flag_add(5, 1);
		} else if (round_no == 2) {
			if (stat("cur", &s) != 0)
				flag_add(3, 1);
			else if (s.st_nlink != 1 && s.st_nlink != 2)
				flag_add(4, 1);
			flag_add(5, 1);
		} else {
			r = readlink("sl", buf, sizeof buf);
			if (r < 0) {
				if (errno != ENOENT)
					flag_add(3, 1);
			} else {
				if (r != 200)
					flag_add(4, 1);
				for (i = 1; i < r; i++)
					if (buf[i] != buf[0])
						flag_add(4, 1);
				flag_add(5, 1);
			}
			if (lstat("sl", &s) != 0) {
				if (errno != ENOENT)
					flag_add(3, 1);
			} else if (!S_ISLNK(s.st_mode)) {
				flag_add(4, 1);
			}
		}
	}
	return NULL;
}

int main(void)
{
	pthread_t c, l1, l2;
	int k;

	pc_begin("XSH.2.9-threads.033");
	guard(120);
	pc_make_file("cur", "x");
	pc_test();
	for (k = 1; k <= 3; k++) {
		int f;

		round_no = k;
		for (f = 1; f <= 5; f++)
			flag_set(f, 0);
		l1 = start(looker, NULL);
		l2 = start(looker, NULL);
		c = start(changer, NULL);
		NEED(pthread_join(c, NULL) == 0 && pthread_join(l1, NULL) == 0 && pthread_join(l2, NULL) == 0, "pthread_join failed");
		NEED(flag_get(2) == 0, "round %d: a changing call failed", k);
		NEED(flag_get(5) >= 50, "round %d: the looking thread made fewer than 50 observations", k);
		pc_check(flag_get(3) == 0, "round %d: a name that must always exist (or a lookup that may only fail with ENOENT) gave another outcome", k);
		pc_check(flag_get(4) == 0, "round %d: a lookup saw a state that no complete call leaves behind", k);
	}
	(void)unlink("cur");
	pc_pass();
}
