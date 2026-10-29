/*
 * The default search directories are directories where they exist.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.xdg.004
 */
#include <sys/stat.h>
#include <unistd.h>
#include "pc.h"
#include "../../../../lib/cv.h"

int main(void)
{
	static const char *const optional[] = { "/usr/local/share", "/etc/xdg", NULL };
	const char *root;
	char p[1024];
	struct stat st;
	int i;

	pc_begin("CONV.xdg.004");
	root = cv_env("ROOT");
	if (root == NULL)
		root = "";
	pc_test();
	(void)snprintf(p, sizeof p, "%s/usr/share", root);
	pc_check(stat(p, &st) == 0 && S_ISDIR(st.st_mode), "%s is not a directory", p);
	for (i = 0; optional[i] != NULL; i++) {
		(void)snprintf(p, sizeof p, "%s%s", root, optional[i]);
		if (stat(p, &st) == 0)
			pc_check(S_ISDIR(st.st_mode), "%s exists but is not a directory", p);
	}
	pc_pass();
}
