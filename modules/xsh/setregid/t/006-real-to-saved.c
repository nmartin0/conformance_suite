/*
 * setregid() without privileges may set the real group ID to the saved
 * set-group-ID from exec.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setregid.006
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* IDs given to the process, one it never holds (probes), and a refused target. */
#define ID_A 65534
#define ID_B 65533
#define ID_C 65532
#define ID_D 65531

#include <fcntl.h>
#include <sys/stat.h>

/* Exit codes of the new image. */
enum { I_OK, I_NOSGID = 10, I_PREP, I_PRIV, I_EPERM = 20, I_OTHER, I_UNCHANGED };

/* The image started from the set-group-ID copy, as user and group 65534. */
static int image(void)
{
	pid_t g;
	int st;

	if (getgid() != ID_A || getegid() != ID_B || getuid() != ID_A || geteuid() != ID_A)
		return I_NOSGID;
	if (setegid(ID_A) != 0 || getegid() != ID_A)
		return I_PREP;
	/* No privilege for setregid(): a change to 65532 must be refused. */
	g = fork();
	if (g == -1)
		return I_PREP;
	if (g == 0)
		_exit(setregid((gid_t)-1, ID_C) == -1 && errno == EPERM ? 0 : 1);
	if (waitpid(g, &st, 0) != g || !WIFEXITED(st))
		return I_PREP;
	if (WEXITSTATUS(st) != 0)
		return I_PRIV;
	if (setregid(ID_B, (gid_t)-1) != 0)
		return errno == EPERM ? I_EPERM : I_OTHER;
	return getgid() == ID_B ? I_OK : I_UNCHANGED;
}

/* A copy of the test program, owned by group 65533, mode 02755 (setup). */
static void make_copy(const char *self)
{
	char buf[8192];
	ssize_t n;
	int in = open(self, O_RDONLY), out;

	pc_check(in >= 0, "cannot open %s: %s", self, pc_errname(errno));
	out = open("sg", O_WRONLY | O_CREAT | O_TRUNC, 0700);
	pc_check(out >= 0, "cannot create a copy: %s", pc_errname(errno));
	while ((n = read(in, buf, sizeof buf)) > 0)
		pc_check(write(out, buf, (size_t)n) == n, "cannot write the copy: %s", pc_errname(errno));
	pc_check(n == 0, "cannot read %s", self);
	pc_check(close(out) == 0 && close(in) == 0, "close failed");
	pc_check(chown("sg", 0, ID_B) == 0, "chown(copy, 0, 65533) failed: %s", pc_errname(errno));
	pc_check(chmod("sg", 02755) == 0, "chmod(copy, 02755) failed: %s", pc_errname(errno));
	pc_check(chmod(".", 0755) == 0, "chmod(.) failed: %s", pc_errname(errno));
}

int main(int argc, char *argv[])
{
	pid_t kid;
	int st, code;

	if (argc == 2 && strcmp(argv[1], "sgid-image") == 0)
		_exit(image());
	pc_begin("XSH.setregid.006");
	if (getuid() != 0 || geteuid() != 0 || getgid() != 0 || getegid() != 0)
		pc_unresolved("needs a test process whose user and group IDs are all 0");
	pc_check(argc >= 1 && argv[0][0] == '/', "argv[0] is not an absolute path");
	make_copy(argv[0]);
	pc_test();
	kid = fork();
	if (kid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (kid == 0) {
		if (setgid(ID_A) != 0 || setuid(ID_A) != 0)
			_exit(I_PREP);
		execl("./sg", "./sg", "sgid-image", (char *)NULL);
		_exit(I_PREP);
	}
	while (waitpid(kid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (!WIFEXITED(st))
		pc_fail("the new image ended abnormally (status %#x)", (unsigned)st);
	code = WEXITSTATUS(st);
	if (code == I_NOSGID)
		pc_unresolved("the copy did not start with effective group ID 65533 (S_ISGID not honoured here)");
	if (code == I_PRIV)
		pc_unresolved("the new image still has appropriate privileges for setregid()");
	if (code == I_PREP)
		pc_unresolved("the new image could not be prepared (setgid(), setuid(), exec or setegid() failed)");
	pc_check(code != I_EPERM, "setregid(<saved set-group-ID from exec>, -1) without privileges failed with EPERM");
	pc_check(code != I_OTHER, "setregid(<saved set-group-ID from exec>, -1) failed with an error other than EPERM");
	pc_check(code != I_UNCHANGED, "setregid(<saved set-group-ID>, -1) returned 0 but getgid() is not 65533");
	pc_check(code == I_OK, "the new image exited with status %d", code);
	pc_pass();
}
