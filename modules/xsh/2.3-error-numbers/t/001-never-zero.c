/*
 * No function sets errno to zero: successful calls leave it non-zero when it was.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.3-error-numbers.001
 */
#include "pc.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <locale.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/*
 * Set errno to a non-zero value, run the call; the call must succeed (else
 * the test cannot tell anything) and errno must not be zero afterwards.
 */
#define T(name, expr) do { \
	int ok_, e_; \
	errno = ERANGE; \
	ok_ = (expr) ? 1 : 0; \
	e_ = errno; \
	NEED(ok_, "%s did not succeed (errno %s)", name, pc_errname(e_)); \
	pc_check(e_ != 0, "%s set errno to zero", name); \
} while (0)

int main(void)
{
	char buf[256];
	int fd = -1, fd2 = -1, p[2], k;
	struct stat st;
	DIR *d = NULL;
	struct dirent *ent;
	FILE *r = NULL, *w = NULL;
	char *m, *end;
	sigset_t none;
	struct timespec ts = { 0, 1 };
	int c, e;

	pc_begin("XSH.2.3-error-numbers.001");
	pc_make_file("f", "hello\nworld\n");
	pc_make_file("g", "x");
	sigemptyset(&none);
	pc_test();
	T("getpid", getpid() > 0);
	T("getcwd", getcwd(buf, sizeof buf) != NULL);
	T("open", (fd = open("f", O_RDONLY)) >= 0);
	T("read", read(fd, buf, 3) == 3);
	T("lseek", lseek(fd, 0, SEEK_SET) == 0);
	T("fstat", fstat(fd, &st) == 0);
	T("stat", stat("f", &st) == 0);
	T("fcntl", fcntl(fd, F_GETFD) >= 0);
	T("dup", (fd2 = dup(fd)) >= 0);
	T("close", close(fd2) == 0);
	T("pipe", pipe(p) == 0);
	T("write", write(p[1], "ab", 2) == 2);
	(void)close(p[0]);
	(void)close(p[1]);
	(void)close(fd);
	T("unlink", unlink("g") == 0);
	T("mkdir", mkdir("d2", 0700) == 0);
	T("rmdir", rmdir("d2") == 0);
	T("opendir", (d = opendir(".")) != NULL);
	for (k = 0; k < 100; k++) {
		errno = ERANGE;
		ent = readdir(d);
		e = errno;
		pc_check(e != 0, "readdir set errno to zero%s", ent == NULL ? " at the end of the directory" : "");
		if (ent == NULL)
			break;
	}
	T("closedir", closedir(d) == 0);
	T("fopen", (r = fopen("f", "r")) != NULL);
	T("fgets", fgets(buf, sizeof buf, r) != NULL);
	for (;;) {
		errno = ERANGE;
		c = fgetc(r);
		e = errno;
		pc_check(e != 0, "fgetc set errno to zero%s", c == EOF ? " at end-of-file" : "");
		if (c == EOF)
			break;
	}
	T("fclose", fclose(r) == 0);
	T("fopen (write)", (w = fopen("out", "w")) != NULL);
	T("fputs", fputs("text", w) >= 0);
	T("fprintf", fprintf(w, "%d", 12) > 0);
	T("fclose (write)", fclose(w) == 0);
	T("malloc", (m = malloc(100)) != NULL);
	T("free", (free(m), 1));
	T("strtol", strtol("12", &end, 10) == 12);
	T("strtod", strtod("1.5", &end) == 1.5);
	T("time", time(NULL) != (time_t)-1);
	T("clock_gettime", clock_gettime(CLOCK_REALTIME, &ts) == 0);
	ts.tv_sec = 0;
	ts.tv_nsec = 1;
	T("nanosleep", nanosleep(&ts, NULL) == 0);
	T("sysconf", sysconf(_SC_PAGESIZE) > 0);
	T("sigprocmask", sigprocmask(SIG_BLOCK, &none, NULL) == 0);
	T("setlocale", setlocale(LC_ALL, "C") != NULL);
	T("getenv", (getenv("PATH"), 1));
	pc_pass();
}
