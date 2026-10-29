/*
 * The per-user default directories can be made under HOME with mode 0700.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.xdg.003
 * MUTANT mkdir.loose
 */
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "pc.h"

static const char *const defaults[] = {
	".config", ".local", ".local/share", ".local/state", ".cache", NULL
};

int main(void)
{
	const char *home;
	char p[1024];
	struct stat st;
	int i, fd;

	pc_begin("CONV.xdg.003");
	home = getenv("HOME");
	pc_check(home != NULL && home[0] == '/', "HOME is not an absolute path");
	/* the specification's variables are unset: the defaults apply */
	(void)unsetenv("XDG_CONFIG_HOME");
	(void)unsetenv("XDG_DATA_HOME");
	(void)unsetenv("XDG_STATE_HOME");
	(void)unsetenv("XDG_CACHE_HOME");
	(void)umask(022);
	pc_test();
	for (i = 0; defaults[i] != NULL; i++) {
		(void)snprintf(p, sizeof p, "%s/%s", home, defaults[i]);
		pc_check(mkdir(p, 0700) == 0, "cannot create $HOME/%s: errno %d", defaults[i], errno);
		pc_check(stat(p, &st) == 0 && S_ISDIR(st.st_mode), "$HOME/%s is not a directory", defaults[i]);
		pc_check((st.st_mode & 07777) == 0700, "$HOME/%s has mode %04o, not 0700", defaults[i],
		    (unsigned)(st.st_mode & 07777));
	}
	/* a file can be created in each, by an application's own subdirectory */
	for (i = 0; defaults[i] != NULL; i++) {
		(void)snprintf(p, sizeof p, "%s/%s/pcconv", home, defaults[i]);
		pc_check(mkdir(p, 0700) == 0, "cannot create a subdirectory of $HOME/%s", defaults[i]);
		(void)strcat(p, "/file");
		fd = open(p, O_CREAT | O_EXCL | O_WRONLY, 0600);
		pc_check(fd >= 0, "cannot create a file in $HOME/%s/pcconv", defaults[i]);
		(void)close(fd);
	}
	/* an existing directory keeps its permissions when the application finds it */
	(void)snprintf(p, sizeof p, "%s/existing", home);
	pc_check(mkdir(p, 0755) == 0, "cannot create a directory");
	pc_check(mkdir(p, 0700) == -1 && errno == EEXIST, "mkdir of an existing directory did not report EEXIST");
	pc_check(stat(p, &st) == 0 && (st.st_mode & 0777) == 0755, "an existing directory's mode changed");
	pc_pass();
}
