/*
 * pcpty - run a command on a private pseudo-terminal.
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * usage: pcpty [-t SECS] [-i INPUT] [-d MS] -o OUTPUT -- CMD [ARG...]
 *
 *   -t SECS   kill the command's session after SECS seconds (default 20)
 *   -i INPUT  file whose bytes are typed at the terminal, one line at a
 *             time with -d milliseconds between lines (default 200), the
 *             first after the same delay
 *   -o OUTPUT file that receives everything the command writes to the
 *             terminal, byte for byte
 *
 * The command gets the slave side as standard input, output and error and
 * as its controlling terminal; the terminal is created here, so no real
 * terminal is ever touched. The exit status is the command's, 124 when the
 * timeout expired and 125 if the terminal could not be set up (a test then
 * reports UNRESOLVED). Built with the strict feature macros: POSIX only.
 */
#define _POSIX_C_SOURCE 202405L
#define _XOPEN_SOURCE 800
#include <sys/types.h>
#include <sys/wait.h>

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

static long now_ms(void)
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec * 1000L + ts.tv_nsec / 1000000L;
}

int main(int argc, char **argv)
{
	const char *inp = NULL, *outp = NULL;
	long secs = 20, delay = 200;
	char *input = NULL;
	size_t inlen = 0, inpos = 0;
	int c, m, s, of, status = 0, done = 0, timed_out = 0;
	char *sname;
	pid_t pid;
	long start, nextin;

	while ((c = getopt(argc, argv, "t:i:d:o:")) != -1) {
		switch (c) {
		case 't': secs = atol(optarg); break;
		case 'i': inp = optarg; break;
		case 'd': delay = atol(optarg); break;
		case 'o': outp = optarg; break;
		default: return 125;
		}
	}
	if (outp == NULL || optind >= argc)
		return 125;
	if (inp != NULL) {
		FILE *f = fopen(inp, "rb");
		char buf[4096];
		size_t n;

		if (f == NULL)
			return 125;
		while ((n = fread(buf, 1, sizeof buf, f)) > 0) {
			input = realloc(input, inlen + n);
			if (input == NULL)
				return 125;
			memcpy(input + inlen, buf, n);
			inlen += n;
		}
		fclose(f);
	}
	of = open(outp, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (of < 0)
		return 125;
	m = posix_openpt(O_RDWR | O_NOCTTY);
	if (m < 0 || grantpt(m) < 0 || unlockpt(m) < 0 || (sname = ptsname(m)) == NULL)
		return 125;
	fflush(NULL);
	pid = fork();
	if (pid < 0)
		return 125;
	if (pid == 0) {
		close(m);
		close(of);
		if (setsid() < 0)
			_exit(125);
		s = open(sname, O_RDWR);	/* becomes the controlling terminal */
		if (s < 0)
			_exit(125);
		if (dup2(s, 0) < 0 || dup2(s, 1) < 0 || dup2(s, 2) < 0)
			_exit(125);
		if (s > 2)
			close(s);
		execvp(argv[optind], argv + optind);
		_exit(127);
	}
	start = now_ms();
	nextin = start + delay;
	while (!done) {
		struct pollfd pf;
		char buf[4096];
		long t = now_ms();
		int w;

		if (secs > 0 && t - start > secs * 1000L) {
			timed_out = 1;
			kill(-pid, SIGKILL);
			kill(pid, SIGKILL);
			break;
		}
		if (inpos < inlen && t >= nextin) {
			size_t e = inpos;

			while (e < inlen && input[e] != '\n')
				e++;
			if (e < inlen)
				e++;
			(void)!write(m, input + inpos, e - inpos);
			inpos = e;
			nextin = now_ms() + delay;
		}
		pf.fd = m;
		pf.events = POLLIN;
		w = poll(&pf, 1, 50);
		if (w > 0 && (pf.revents & POLLIN)) {
			ssize_t n = read(m, buf, sizeof buf);

			if (n > 0)
				(void)!write(of, buf, (size_t)n);
		}
		if (waitpid(pid, &status, WNOHANG) == pid) {
			/* drain what is left */
			for (;;) {
				ssize_t n;

				pf.fd = m;
				pf.events = POLLIN;
				if (poll(&pf, 1, 100) <= 0 || !(pf.revents & POLLIN))
					break;
				n = read(m, buf, sizeof buf);
				if (n <= 0)
					break;
				(void)!write(of, buf, (size_t)n);
			}
			done = 1;
		}
	}
	if (timed_out) {
		waitpid(pid, &status, 0);
		return 124;
	}
	close(of);
	if (WIFEXITED(status))
		return WEXITSTATUS(status);
	return 128 + (WIFSIGNALED(status) ? WTERMSIG(status) : 0);
}
