/*
 * Without WRDE_SHOWERR the standard error of a command run for command substitution goes to /dev/null.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wordexp.021
 */
#include "wx.h"

#include <fcntl.h>

int main(void)
{
	wordexp_t w;
	int fd, save, r;
	size_t n;
	char *word;

	pc_begin("XSH.wordexp.021");
	fd = open("wx.err", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	pc_check(fd >= 0, "cannot create the capture file");
	save = dup(2);
	pc_check(save >= 0, "dup failed");
	pc_test();
	memset(&w, 0, sizeof w);
	pc_check(dup2(fd, 2) == 2, "dup2 failed");
	r = wordexp("$(echo oops >&2; echo x)", &w, 0);
	n = w.we_wordc;
	word = (r == 0 && n == 1) ? w.we_wordv[0] : NULL;
	(void)dup2(save, 2);
	pc_check(r == 0, "wordexp returned %d", r);
	pc_check(word != NULL && strcmp(word, "x") == 0, "the command did not run as expected");
	pc_check(pc_file_size("wx.err") == 0, "the command's standard error reached the process's");
	wordfree(&w);
	close(fd);
	close(save);
	(void)unlink("wx.err");
	pc_pass();
}
