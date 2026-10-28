/*
 * Directory operations that fail leave the file system unchanged: the same
 * names, files, link counts and sizes before and after.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.4-directory-operations.003
 */
#include "pc.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAXL 256

static char *lines[MAXL];
static int nlines;

static int cmpstr(const void *a, const void *b)
{
	return strcmp(*(char *const *)a, *(char *const *)b);
}

/* Records one line per name below dir: path, kind, inode, link count and, for files, size. */
static void walk(const char *dir)
{
	DIR *d = opendir(dir);
	struct dirent *e;
	char path[512], buf[700];
	struct stat sb;

	if (d == NULL)
		pc_unresolved("opendir(%s) failed: %s", dir, pc_errname(errno));
	while ((e = readdir(d)) != NULL) {
		if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0)
			continue;
		(void)snprintf(path, sizeof path, "%s/%s", dir, e->d_name);
		if (lstat(path, &sb) != 0)
			pc_unresolved("lstat(%s) failed: %s", path, pc_errname(errno));
		(void)snprintf(buf, sizeof buf, "%s mode=%o ino=%lu nlink=%lu size=%ld", path,
		    (unsigned)sb.st_mode, (unsigned long)sb.st_ino, (unsigned long)sb.st_nlink,
		    S_ISDIR(sb.st_mode) ? 0L : (long)sb.st_size);
		if (nlines >= MAXL)
			pc_unresolved("the tree is larger than expected");
		lines[nlines] = strdup(buf);
		if (lines[nlines] == NULL)
			pc_unresolved("strdup failed");
		nlines++;
		if (S_ISDIR(sb.st_mode))
			walk(path);
	}
	(void)closedir(d);
}

static char *snapshot(void)
{
	size_t total = 1;
	int i;
	char *out;

	nlines = 0;
	walk("tree");
	qsort(lines, (size_t)nlines, sizeof lines[0], cmpstr);
	for (i = 0; i < nlines; i++)
		total += strlen(lines[i]) + 1;
	out = malloc(total);
	if (out == NULL)
		pc_unresolved("malloc failed");
	out[0] = '\0';
	for (i = 0; i < nlines; i++) {
		strcat(out, lines[i]);
		strcat(out, "\n");
		free(lines[i]);
	}
	return out;
}

static int try_open_excl_trunc(void)
{
	int fd = open("tree/f1", O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, 0600);

	if (fd >= 0)
		(void)close(fd);
	return fd < 0 ? -1 : 0;
}

static int try_mkdir_long(void)
{
	long max = pathconf("tree", _PC_NAME_MAX);
	char nm[600];

	if (max < 0 || max > 400)
		return -2;
	(void)strcpy(nm, "tree/");
	memset(nm + 5, 'n', (size_t)max + 1);
	nm[5 + max + 1] = '\0';
	return mkdir(nm, 0700);
}

int main(void)
{
	char *before, *after;
	int i, r, skipped = 0;
	static const char *what[] = {
		"mkdir with a missing parent", "mkdir of an existing name", "rmdir of a non-empty directory",
		"rename of a directory onto a non-empty directory", "rename of a directory into itself",
		"link onto an existing name", "symlink onto an existing name", "open O_CREAT|O_EXCL|O_TRUNC on an existing file",
		"rename of a file onto a directory", "rename of a directory onto a file", "rename of a missing file",
		"rmdir of a regular file", "mkfifo onto an existing name", "link of a missing file",
		"mkdir below a regular file", "mkdir of a too long name"
	};

	pc_begin("XBD.4.4-directory-operations.003");
	pc_make_dir("tree");
	pc_make_file("tree/f1", "some data\n");
	pc_make_file("tree/f2", "more\n");
	pc_make_dir("tree/dir1");
	pc_make_dir("tree/dir1/sub");
	pc_make_dir("tree/full");
	pc_make_file("tree/full/x", "x");
	pc_make_dir("tree/exist");
	pc_test();
	for (i = 0; i < 16; i++) {
		before = snapshot();
		switch (i) {
		case 0: r = mkdir("tree/a/b/c", 0700); break;
		case 1: r = mkdir("tree/exist", 0700); break;
		case 2: r = rmdir("tree/full"); break;
		case 3: r = rename("tree/dir1", "tree/full"); break;
		case 4: r = rename("tree/dir1", "tree/dir1/sub/in"); break;
		case 5: r = link("tree/f1", "tree/f2"); break;
		case 6: r = symlink("x", "tree/f2"); break;
		case 7: r = try_open_excl_trunc(); break;
		case 8: r = rename("tree/f1", "tree/dir1"); break;
		case 9: r = rename("tree/dir1", "tree/f1"); break;
		case 10: r = rename("tree/none", "tree/f1"); break;
		case 11: r = rmdir("tree/f1"); break;
		case 12: r = mkfifo("tree/f1", 0600); break;
		case 13: r = link("tree/none", "tree/new"); break;
		case 14: r = mkdir("tree/f1/sub", 0700); break;
		default: r = try_mkdir_long(); break;
		}
		if (r == -2) {
			skipped++;	/* no usable {NAME_MAX}: this step does not apply */
			free(before);
			continue;
		}
		pc_check(r != 0, "%s succeeded, but it must fail", what[i]);
		after = snapshot();
		pc_check(strcmp(before, after) == 0, "the file system changed after a failed %s:\n--- before\n%s--- after\n%s",
		    what[i], before, after);
		free(before);
		free(after);
	}
	pc_check(skipped < 16, "no step could run");
	pc_pass();
}
