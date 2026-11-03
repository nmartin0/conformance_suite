#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
static int count(const char *d) { DIR *p = opendir(d); struct dirent *e; int n = 0; if (!p) return -1; while ((e = readdir(p))) n++; closedir(p); return n; }
int main(void)
{
	char name[300]; struct stat st; int fd, i;
	umask(0);
	mkdir("pd_d", 0777); stat("pd_d", &st); printf("mkdir 0777 umask 0 -> mode %o\n", (unsigned)st.st_mode & 07777);
	for (i = 0; i < 3; i++) { char f[32]; sprintf(f, "pd_d/f%d", i); close(creat(f, 0644)); }
	{ DIR *p = opendir("pd_d"); struct dirent *e; int a = 0, b = 0; while ((e = readdir(p))) a++; rewinddir(p); while ((e = readdir(p))) b++; closedir(p); printf("readdir first pass %d, after rewinddir %d\n", a, b); }
	fd = open("pd_d", O_SEARCH | O_DIRECTORY);
	printf("open O_SEARCH dir -> %d (errno %d)\n", fd, fd < 0 ? errno : 0);
	if (fd >= 0) { int r = mkdirat(fd, "sub", 0755); printf("mkdirat via O_SEARCH -> %d errno %d\n", r, r ? errno : 0); r = unlinkat(fd, "f0", 0); printf("unlinkat via O_SEARCH -> %d errno %d\n", r, r ? errno : 0); r = fstatat(fd, "f1", &st, 0); printf("fstatat via O_SEARCH -> %d errno %d\n", r, r ? errno : 0); close(fd); }
	fd = open("pd_d", O_RDONLY | O_DIRECTORY);
	if (fd >= 0) { int r = fstatat(fd, "f1", &st, 0); printf("fstatat via O_RDONLY dir fd -> %d errno %d\n", r, r ? errno : 0); close(fd); }
	memset(name, 'a', 256); name[256] = 0;
	i = stat(name, &st); printf("stat(256 a) -> %d errno %d (ENAMETOOLONG=%d)\n", i, errno, ENAMETOOLONG);
	{ FILE *f = fopen("pd_d/f1", "re"); if (f) { int fl = fcntl(fileno(f), F_GETFD); printf("fopen re: FD_CLOEXEC=%d\n", !!(fl & FD_CLOEXEC)); fclose(f); } else printf("fopen re failed errno %d\n", errno); }
	{ int a = open("pd_d/ap", O_WRONLY | O_CREAT | O_APPEND, 0644); int b = open("pd_d/ap", O_WRONLY | O_APPEND); int k; for (k = 0; k < 100; k++) { write(a, "aaaaaaaaaa", 10); write(b, "bbbbbbbbbb", 10); } close(a); close(b); stat("pd_d/ap", &st); printf("append: size %lld (expect 2000)\n", (long long)st.st_size); }
	{ struct statvfs sv; if (!statvfs(".", &sv)) printf("statvfs: bsize %lu namemax %lu flag 0x%lx\n", sv.f_bsize, sv.f_namemax, sv.f_flag); printf("pathconf NAME_MAX %ld FILESIZEBITS %ld\n", pathconf(".", _PC_NAME_MAX), pathconf(".", _PC_FILESIZEBITS)); }
	printf("rename dir into dir/.: "); mkdir("pd_e", 0755); i = rename("pd_e", "pd_d/."); printf("%d errno %d\n", i, i ? errno : 0);
	return 0;
}
