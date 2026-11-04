#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
static int cnt(DIR *p) { int n = 0; while (readdir(p)) n++; return n; }
static void t(const char *what, int r) { printf("%s -> %d%s%s\n", what, r, r < 0 ? " errno " : "", r < 0 ? strerror(errno) : ""); }
int main(void)
{
	DIR *p; int i;
	mkdir("e", 0755); mkdir("d2", 0755); close(creat("f", 0644));
	t("rename(e, d2/.)", rename("e", "d2/."));
	t("rename(f, nf/)", rename("f", "nf/"));
	mkdir("dd", 0755);
	t("rename(dd, nd/)", rename("dd", "nd/"));
	{ struct stat s; t("stat(f/..)", stat("f/..", &s)); }
	t("open(d2, O_RDONLY|O_CREAT)", open("d2", O_RDONLY | O_CREAT, 0644));
	mkdir("rw", 0755); for (i = 0; i < 3; i++) { char n[16]; sprintf(n, "rw/f%d", i); close(creat(n, 0644)); }
	p = opendir("rw"); printf("rewinddir A: fresh open: first pass %d, ", cnt(p)); rewinddir(p); printf("after rewind %d\n", cnt(p)); closedir(p);
	p = opendir("rw"); rewinddir(p); printf("rewinddir B: rewind before reading: %d\n", cnt(p)); closedir(p);
	p = opendir("rw"); readdir(p); readdir(p); rewinddir(p); printf("rewinddir C: after 2 reads then rewind: %d\n", cnt(p)); closedir(p);
	p = opendir("rw"); { long loc = telldir(p); cnt(p); seekdir(p, loc); printf("seekdir to start after full read: %d\n", cnt(p)); } closedir(p);
	return 0;
}
