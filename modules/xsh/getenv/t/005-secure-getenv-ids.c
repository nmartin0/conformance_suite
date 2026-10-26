/*
 * secure_getenv() returns a null pointer in a process whose real and
 * effective user IDs, or group IDs, differed at startup.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getenv.005
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/* The new image: 1 if getenv() misses PCX_S, 2 if secure_getenv() finds it. */
static int child(void)
{
	if (getenv("PCX_S") == NULL)
		return 1;
	if (secure_getenv("PCX_S") != NULL)
		return 2;
	return 0;
}

/* Run the test again with the effective user (uid) or group (gid) ID set to 65534. */
static int run(const char *path, int gid)
{
	int st;
	pid_t pid = fork();

	if (pid == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (pid == 0) {
		if (gid ? setegid(65534) != 0 : seteuid(65534) != 0)
			_exit(98);
		if (gid ? getgid() == getegid() : getuid() == geteuid())
			_exit(98);
		execv(path, (char *[]){ (char *)path, "child", NULL });
		_exit(99);
	}
	while (waitpid(pid, &st, 0) == -1)
		if (errno != EINTR)
			pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (!WIFEXITED(st) || WEXITSTATUS(st) >= 98 || WEXITSTATUS(st) == 1)
		pc_unresolved("the %s child did not run (the exec may need the test readable and searchable "
			      "for ID 65534) or lost PCX_S (status %#x)", gid ? "group" : "user", (unsigned)st);
	return WEXITSTATUS(st);
}

int main(int argc, char *argv[])
{
	if (argc == 2 && strcmp(argv[1], "child") == 0)
		_exit(child());
	pc_begin("XSH.getenv.005");
	pc_check(argc >= 1 && argv[0][0] == '/', "argv[0] is not an absolute path");
	if (geteuid() != 0)
		pc_unresolved("needs appropriate privileges to set a different effective ID");
	pc_check(setenv("PCX_S", "secret", 1) == 0, "setenv failed: %s", pc_errname(errno));
	pc_test();
	pc_check(run(argv[0], 0) == 0,
		 "secure_getenv() found PCX_S in a process started with real user ID 0 and effective user ID 65534");
	pc_check(run(argv[0], 1) == 0,
		 "secure_getenv() found PCX_S in a process started with real group ID 0 and effective group ID 65534");
	pc_pass();
}
