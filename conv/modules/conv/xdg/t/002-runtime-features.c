/*
 * XDG_RUNTIME_DIR supports the features applications rely on.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.xdg.002
 * MUTANT link.noop
 */
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include "pc.h"
#include "../../../../lib/cv.h"

static char sub[512];
static char base[512];

static void wipe(void)
{
	DIR *dp;
	struct dirent *e;

	if (sub[0] == '\0')
		return;
	if (chdir(sub) == 0 && (dp = opendir(".")) != NULL) {
		while ((e = readdir(dp)) != NULL)
			if (strcmp(e->d_name, ".") != 0 && strcmp(e->d_name, "..") != 0)
				(void)unlink(e->d_name);
		(void)closedir(dp);
	}
	if (chdir(base) == 0)
		(void)rmdir(sub);
}

int main(void)
{
	const char *d;
	struct stat st;
	int fd, i;
	struct sockaddr_un sa;
	int ls, cs;
	char buf[64];
	struct flock fl;
	pid_t pid;
	int status;
	void *m;
	char name[4];

	pc_begin("CONV.xdg.002");
	d = cv_env("XDG_RUNTIME_DIR");
	if (d == NULL)
		pc_notinuse("XDG_RUNTIME_DIR is not set in the environment the suite was started from");
	pc_check(d[0] == '/' && cv_isdir(d), "XDG_RUNTIME_DIR is not an existing absolute directory");
	(void)snprintf(base, sizeof base, "%s", d);
	(void)snprintf(sub, sizeof sub, "%s/pcconv.%ld", d, (long)getpid());
	pc_check(mkdir(sub, 0700) == 0, "cannot make a scratch directory in %s", d);
	(void)atexit(wipe);
	pc_check(chdir(sub) == 0, "cannot enter the scratch directory");
	pc_test();

	/* symbolic link */
	pc_check(symlink("target-text", "sl") == 0, "symlink() failed: errno %d", errno);
	pc_check(readlink("sl", buf, sizeof buf) == 11 && memcmp(buf, "target-text", 11) == 0,
	    "readlink() does not return the link text");

	/* hard link and a reliable link count */
	fd = open("f", O_RDWR | O_CREAT | O_EXCL, 0600);
	pc_check(fd >= 0, "cannot create a file");
	pc_check(fstat(fd, &st) == 0 && st.st_nlink == 1, "a new file has link count != 1");
	pc_check(link("f", "h") == 0, "link() failed: errno %d", errno);
	pc_check(fstat(fd, &st) == 0 && st.st_nlink == 2, "link count after link() is %ld, not 2",
	    (long)st.st_nlink);
	pc_check(unlink("h") == 0, "unlink of the extra link failed");
	pc_check(fstat(fd, &st) == 0 && st.st_nlink == 1, "link count after unlink() is not back to 1");

	/* permissions */
	pc_check(fchmod(fd, 0640) == 0 && fstat(fd, &st) == 0 && (st.st_mode & 0777) == 0640,
	    "mode bits cannot be set and read back");

	/* file locking: a lock held here is seen by another process */
	memset(&fl, 0, sizeof fl);
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	pc_check(fcntl(fd, F_SETLK, &fl) == 0, "cannot take a write lock: errno %d", errno);
	pid = fork();
	pc_check(pid >= 0, "fork failed");
	if (pid == 0) {
		struct flock q;

		memset(&q, 0, sizeof q);
		q.l_type = F_WRLCK;
		q.l_whence = SEEK_SET;
		if (fcntl(fd, F_GETLK, &q) != 0)
			_exit(2);
		_exit(q.l_type == F_UNLCK ? 1 : 0);
	}
	pc_check(waitpid(pid, &status, 0) == pid && WIFEXITED(status), "lock probe did not finish");
	pc_check(WEXITSTATUS(status) == 0, "a record lock is not visible to another process (probe status %d)",
	    WEXITSTATUS(status));

	/* sparse file: a hole costs no storage */
	pc_check(pwrite(fd, "x", 1, (off_t)(64 << 20) - 1) == 1, "cannot write at offset 64 MiB");
	pc_check(fstat(fd, &st) == 0 && st.st_size == (off_t)(64 << 20), "size after a far write is wrong");
	pc_check((long long)st.st_blocks * 512 < (64 << 20) / 2,
	    "a 64 MiB file with one byte written occupies %lld bytes: no sparse files",
	    (long long)st.st_blocks * 512);

	/* memory mapping, coherent with read() */
	m = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	pc_check(m != MAP_FAILED, "mmap(MAP_SHARED) failed: errno %d", errno);
	memcpy(m, "mapped!", 7);
	pc_check(msync(m, 4096, MS_SYNC) == 0, "msync failed");
	pc_check(pread(fd, buf, 7, 0) == 7 && memcmp(buf, "mapped!", 7) == 0,
	    "data stored through a mapping is not seen by read()");
	(void)munmap(m, 4096);
	(void)close(fd);

	/* AF_UNIX socket (relative name: the absolute path may exceed sun_path) */
	ls = socket(AF_UNIX, SOCK_STREAM, 0);
	pc_check(ls >= 0, "socket(AF_UNIX) failed: errno %d", errno);
	memset(&sa, 0, sizeof sa);
	sa.sun_family = AF_UNIX;
	(void)strcpy(sa.sun_path, "sock");
	pc_check(bind(ls, (struct sockaddr *)&sa, sizeof sa) == 0, "bind() of a socket file failed: errno %d", errno);
	pc_check(stat("sock", &st) == 0 && S_ISSOCK(st.st_mode), "bind() did not create a socket file");
	pc_check(listen(ls, 1) == 0, "listen failed");
	cs = socket(AF_UNIX, SOCK_STREAM, 0);
	pc_check(cs >= 0 && connect(cs, (struct sockaddr *)&sa, sizeof sa) == 0,
	    "connect() to the socket file failed: errno %d", errno);
	(void)close(cs);
	(void)close(ls);

	/* any byte but '/' and NUL in a file name */
	name[0] = 'x';
	name[2] = 'x';
	name[3] = '\0';
	for (i = 1; i < 256; i++) {
		if (i == '/')
			continue;
		name[1] = (char)i;
		fd = open(name, O_CREAT | O_EXCL | O_WRONLY, 0600);
		pc_check(fd >= 0, "cannot create a file whose name contains byte 0x%02x: errno %d", i, errno);
		(void)close(fd);
		pc_check(stat(name, &st) == 0 && unlink(name) == 0, "file with byte 0x%02x in its name cannot be used", i);
	}
	pc_pass();
}
