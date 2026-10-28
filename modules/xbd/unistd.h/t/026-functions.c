/*
 * <unistd.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.026
 *
 * Only <unistd.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <unistd.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.unistd.h.026");
	pc_test();
	if (!IS_T(&(access), int (*)(const char *, int)))
		pc_fail("access() is not declared as int access(const char *, int)");
	if (IS_T(&(access), int (*)(const char *, int, int)))
		pc_fail("access() is declared without a prototype");
	if (!IS_T(&(alarm), unsigned (*)(unsigned)))
		pc_fail("alarm() is not declared as unsigned alarm(unsigned)");
	if (IS_T(&(alarm), unsigned (*)(unsigned, int)))
		pc_fail("alarm() is declared without a prototype");
	if (!IS_T(&(chdir), int (*)(const char *)))
		pc_fail("chdir() is not declared as int chdir(const char *)");
	if (IS_T(&(chdir), int (*)(const char *, int)))
		pc_fail("chdir() is declared without a prototype");
	if (!IS_T(&(chown), int (*)(const char *, uid_t, gid_t)))
		pc_fail("chown() is not declared as int chown(const char *, uid_t, gid_t)");
	if (IS_T(&(chown), int (*)(const char *, uid_t, gid_t, int)))
		pc_fail("chown() is declared without a prototype");
	if (!IS_T(&(close), int (*)(int)))
		pc_fail("close() is not declared as int close(int)");
	if (IS_T(&(close), int (*)(int, int)))
		pc_fail("close() is declared without a prototype");
	if (!IS_T(&(confstr), size_t (*)(int, char *, size_t)))
		pc_fail("confstr() is not declared as size_t confstr(int, char *, size_t)");
	if (IS_T(&(confstr), size_t (*)(int, char *, size_t, int)))
		pc_fail("confstr() is declared without a prototype");
	if (!IS_T(&(dup), int (*)(int)))
		pc_fail("dup() is not declared as int dup(int)");
	if (IS_T(&(dup), int (*)(int, int)))
		pc_fail("dup() is declared without a prototype");
	if (!IS_T(&(dup2), int (*)(int, int)))
		pc_fail("dup2() is not declared as int dup2(int, int)");
	if (IS_T(&(dup2), int (*)(int, int, int)))
		pc_fail("dup2() is declared without a prototype");
	if (!IS_T(&(_exit), void (*)(int)))
		pc_fail("_exit() is not declared as _Noreturn void _exit(int)");
	if (IS_T(&(_exit), void (*)(int, int)))
		pc_fail("_exit() is declared without a prototype");
	if (!IS_T(&(execl), int (*)(const char *, const char *, ...)))
		pc_fail("execl() is not declared as int execl(const char *, const char *, ...)");
	if (!IS_T(&(execle), int (*)(const char *, const char *, ...)))
		pc_fail("execle() is not declared as int execle(const char *, const char *, ...)");
	if (!IS_T(&(execlp), int (*)(const char *, const char *, ...)))
		pc_fail("execlp() is not declared as int execlp(const char *, const char *, ...)");
	if (!IS_T(&(execv), int (*)(const char *, char *const [])))
		pc_fail("execv() is not declared as int execv(const char *, char *const [])");
	if (IS_T(&(execv), int (*)(const char *, char *const [], int)))
		pc_fail("execv() is declared without a prototype");
	if (!IS_T(&(execve), int (*)(const char *, char *const [], char *const [])))
		pc_fail("execve() is not declared as int execve(const char *, char *const [], char *const [])");
	if (IS_T(&(execve), int (*)(const char *, char *const [], char *const [], int)))
		pc_fail("execve() is declared without a prototype");
	if (!IS_T(&(execvp), int (*)(const char *, char *const [])))
		pc_fail("execvp() is not declared as int execvp(const char *, char *const [])");
	if (IS_T(&(execvp), int (*)(const char *, char *const [], int)))
		pc_fail("execvp() is declared without a prototype");
	if (!IS_T(&(faccessat), int (*)(int, const char *, int, int)))
		pc_fail("faccessat() is not declared as int faccessat(int, const char *, int, int)");
	if (IS_T(&(faccessat), int (*)(int, const char *, int, int, int)))
		pc_fail("faccessat() is declared without a prototype");
	if (!IS_T(&(fchdir), int (*)(int)))
		pc_fail("fchdir() is not declared as int fchdir(int)");
	if (IS_T(&(fchdir), int (*)(int, int)))
		pc_fail("fchdir() is declared without a prototype");
	if (!IS_T(&(fchown), int (*)(int, uid_t, gid_t)))
		pc_fail("fchown() is not declared as int fchown(int, uid_t, gid_t)");
	if (IS_T(&(fchown), int (*)(int, uid_t, gid_t, int)))
		pc_fail("fchown() is declared without a prototype");
	if (!IS_T(&(fchownat), int (*)(int, const char *, uid_t, gid_t, int)))
		pc_fail("fchownat() is not declared as int fchownat(int, const char *, uid_t, gid_t, int)");
	if (IS_T(&(fchownat), int (*)(int, const char *, uid_t, gid_t, int, int)))
		pc_fail("fchownat() is declared without a prototype");
	if (!IS_T(&(fexecve), int (*)(int, char *const [], char *const [])))
		pc_fail("fexecve() is not declared as int fexecve(int, char *const [], char *const [])");
	if (IS_T(&(fexecve), int (*)(int, char *const [], char *const [], int)))
		pc_fail("fexecve() is declared without a prototype");
	if (!IS_T(&(fork), pid_t (*)(void)))
		pc_fail("fork() is not declared as pid_t fork(void)");
	if (IS_T(&(fork), pid_t (*)(int)))
		pc_fail("fork() is declared without a prototype");
	if (!IS_T(&(fpathconf), long (*)(int, int)))
		pc_fail("fpathconf() is not declared as long fpathconf(int, int)");
	if (IS_T(&(fpathconf), long (*)(int, int, int)))
		pc_fail("fpathconf() is declared without a prototype");
	if (!IS_T(&(ftruncate), int (*)(int, off_t)))
		pc_fail("ftruncate() is not declared as int ftruncate(int, off_t)");
	if (IS_T(&(ftruncate), int (*)(int, off_t, int)))
		pc_fail("ftruncate() is declared without a prototype");
	if (!IS_T(&(getcwd), char *(*)(char *, size_t)))
		pc_fail("getcwd() is not declared as char *getcwd(char *, size_t)");
	if (IS_T(&(getcwd), char *(*)(char *, size_t, int)))
		pc_fail("getcwd() is declared without a prototype");
	if (!IS_T(&(getegid), gid_t (*)(void)))
		pc_fail("getegid() is not declared as gid_t getegid(void)");
	if (IS_T(&(getegid), gid_t (*)(int)))
		pc_fail("getegid() is declared without a prototype");
	if (!IS_T(&(geteuid), uid_t (*)(void)))
		pc_fail("geteuid() is not declared as uid_t geteuid(void)");
	if (IS_T(&(geteuid), uid_t (*)(int)))
		pc_fail("geteuid() is declared without a prototype");
	if (!IS_T(&(getgid), gid_t (*)(void)))
		pc_fail("getgid() is not declared as gid_t getgid(void)");
	if (IS_T(&(getgid), gid_t (*)(int)))
		pc_fail("getgid() is declared without a prototype");
	if (!IS_T(&(getgroups), int (*)(int, gid_t [])))
		pc_fail("getgroups() is not declared as int getgroups(int, gid_t [])");
	if (IS_T(&(getgroups), int (*)(int, gid_t [], int)))
		pc_fail("getgroups() is declared without a prototype");
	if (!IS_T(&(gethostname), int (*)(char *, size_t)))
		pc_fail("gethostname() is not declared as int gethostname(char *, size_t)");
	if (IS_T(&(gethostname), int (*)(char *, size_t, int)))
		pc_fail("gethostname() is declared without a prototype");
	if (!IS_T(&(getlogin), char *(*)(void)))
		pc_fail("getlogin() is not declared as char *getlogin(void)");
	if (IS_T(&(getlogin), char *(*)(int)))
		pc_fail("getlogin() is declared without a prototype");
	if (!IS_T(&(getlogin_r), int (*)(char *, size_t)))
		pc_fail("getlogin_r() is not declared as int getlogin_r(char *, size_t)");
	if (IS_T(&(getlogin_r), int (*)(char *, size_t, int)))
		pc_fail("getlogin_r() is declared without a prototype");
	if (!IS_T(&(getopt), int (*)(int, char * const [], const char *)))
		pc_fail("getopt() is not declared as int getopt(int, char * const [], const char *)");
	if (IS_T(&(getopt), int (*)(int, char * const [], const char *, int)))
		pc_fail("getopt() is declared without a prototype");
	if (!IS_T(&(getpgid), pid_t (*)(pid_t)))
		pc_fail("getpgid() is not declared as pid_t getpgid(pid_t)");
	if (IS_T(&(getpgid), pid_t (*)(pid_t, int)))
		pc_fail("getpgid() is declared without a prototype");
	if (!IS_T(&(getpgrp), pid_t (*)(void)))
		pc_fail("getpgrp() is not declared as pid_t getpgrp(void)");
	if (IS_T(&(getpgrp), pid_t (*)(int)))
		pc_fail("getpgrp() is declared without a prototype");
	if (!IS_T(&(getpid), pid_t (*)(void)))
		pc_fail("getpid() is not declared as pid_t getpid(void)");
	if (IS_T(&(getpid), pid_t (*)(int)))
		pc_fail("getpid() is declared without a prototype");
	if (!IS_T(&(getppid), pid_t (*)(void)))
		pc_fail("getppid() is not declared as pid_t getppid(void)");
	if (IS_T(&(getppid), pid_t (*)(int)))
		pc_fail("getppid() is declared without a prototype");
	if (!IS_T(&(getsid), pid_t (*)(pid_t)))
		pc_fail("getsid() is not declared as pid_t getsid(pid_t)");
	if (IS_T(&(getsid), pid_t (*)(pid_t, int)))
		pc_fail("getsid() is declared without a prototype");
	if (!IS_T(&(getuid), uid_t (*)(void)))
		pc_fail("getuid() is not declared as uid_t getuid(void)");
	if (IS_T(&(getuid), uid_t (*)(int)))
		pc_fail("getuid() is declared without a prototype");
	if (!IS_T(&(isatty), int (*)(int)))
		pc_fail("isatty() is not declared as int isatty(int)");
	if (IS_T(&(isatty), int (*)(int, int)))
		pc_fail("isatty() is declared without a prototype");
	if (!IS_T(&(lchown), int (*)(const char *, uid_t, gid_t)))
		pc_fail("lchown() is not declared as int lchown(const char *, uid_t, gid_t)");
	if (IS_T(&(lchown), int (*)(const char *, uid_t, gid_t, int)))
		pc_fail("lchown() is declared without a prototype");
	if (!IS_T(&(link), int (*)(const char *, const char *)))
		pc_fail("link() is not declared as int link(const char *, const char *)");
	if (IS_T(&(link), int (*)(const char *, const char *, int)))
		pc_fail("link() is declared without a prototype");
	if (!IS_T(&(linkat), int (*)(int, const char *, int, const char *, int)))
		pc_fail("linkat() is not declared as int linkat(int, const char *, int, const char *, int)");
	if (IS_T(&(linkat), int (*)(int, const char *, int, const char *, int, int)))
		pc_fail("linkat() is declared without a prototype");
	if (!IS_T(&(lseek), off_t (*)(int, off_t, int)))
		pc_fail("lseek() is not declared as off_t lseek(int, off_t, int)");
	if (IS_T(&(lseek), off_t (*)(int, off_t, int, int)))
		pc_fail("lseek() is declared without a prototype");
	if (!IS_T(&(pathconf), long (*)(const char *, int)))
		pc_fail("pathconf() is not declared as long pathconf(const char *, int)");
	if (IS_T(&(pathconf), long (*)(const char *, int, int)))
		pc_fail("pathconf() is declared without a prototype");
	if (!IS_T(&(pause), int (*)(void)))
		pc_fail("pause() is not declared as int pause(void)");
	if (IS_T(&(pause), int (*)(int)))
		pc_fail("pause() is declared without a prototype");
	if (!IS_T(&(pipe), int (*)(int [2])))
		pc_fail("pipe() is not declared as int pipe(int [2])");
	if (IS_T(&(pipe), int (*)(int [2], int)))
		pc_fail("pipe() is declared without a prototype");
	if (!IS_T(&(pread), ssize_t (*)(int, void *, size_t, off_t)))
		pc_fail("pread() is not declared as ssize_t pread(int, void *, size_t, off_t)");
	if (IS_T(&(pread), ssize_t (*)(int, void *, size_t, off_t, int)))
		pc_fail("pread() is declared without a prototype");
	if (!IS_T(&(pwrite), ssize_t (*)(int, const void *, size_t, off_t)))
		pc_fail("pwrite() is not declared as ssize_t pwrite(int, const void *, size_t, off_t)");
	if (IS_T(&(pwrite), ssize_t (*)(int, const void *, size_t, off_t, int)))
		pc_fail("pwrite() is declared without a prototype");
	if (!IS_T(&(read), ssize_t (*)(int, void *, size_t)))
		pc_fail("read() is not declared as ssize_t read(int, void *, size_t)");
	if (IS_T(&(read), ssize_t (*)(int, void *, size_t, int)))
		pc_fail("read() is declared without a prototype");
	if (!IS_T(&(readlink), ssize_t (*)(const char *restrict, char *restrict, size_t)))
		pc_fail("readlink() is not declared as ssize_t readlink(const char *restrict, char *restrict, size_t)");
	if (IS_T(&(readlink), ssize_t (*)(const char *restrict, char *restrict, size_t, int)))
		pc_fail("readlink() is declared without a prototype");
	if (!IS_T(&(readlinkat), ssize_t (*)(int, const char *restrict, char *restrict, size_t)))
		pc_fail("readlinkat() is not declared as ssize_t readlinkat(int, const char *restrict, char *restrict, size_t)");
	if (IS_T(&(readlinkat), ssize_t (*)(int, const char *restrict, char *restrict, size_t, int)))
		pc_fail("readlinkat() is declared without a prototype");
	if (!IS_T(&(rmdir), int (*)(const char *)))
		pc_fail("rmdir() is not declared as int rmdir(const char *)");
	if (IS_T(&(rmdir), int (*)(const char *, int)))
		pc_fail("rmdir() is declared without a prototype");
	if (!IS_T(&(setegid), int (*)(gid_t)))
		pc_fail("setegid() is not declared as int setegid(gid_t)");
	if (IS_T(&(setegid), int (*)(gid_t, int)))
		pc_fail("setegid() is declared without a prototype");
	if (!IS_T(&(seteuid), int (*)(uid_t)))
		pc_fail("seteuid() is not declared as int seteuid(uid_t)");
	if (IS_T(&(seteuid), int (*)(uid_t, int)))
		pc_fail("seteuid() is declared without a prototype");
	if (!IS_T(&(setgid), int (*)(gid_t)))
		pc_fail("setgid() is not declared as int setgid(gid_t)");
	if (IS_T(&(setgid), int (*)(gid_t, int)))
		pc_fail("setgid() is declared without a prototype");
	if (!IS_T(&(setpgid), int (*)(pid_t, pid_t)))
		pc_fail("setpgid() is not declared as int setpgid(pid_t, pid_t)");
	if (IS_T(&(setpgid), int (*)(pid_t, pid_t, int)))
		pc_fail("setpgid() is declared without a prototype");
	if (!IS_T(&(setsid), pid_t (*)(void)))
		pc_fail("setsid() is not declared as pid_t setsid(void)");
	if (IS_T(&(setsid), pid_t (*)(int)))
		pc_fail("setsid() is declared without a prototype");
	if (!IS_T(&(setuid), int (*)(uid_t)))
		pc_fail("setuid() is not declared as int setuid(uid_t)");
	if (IS_T(&(setuid), int (*)(uid_t, int)))
		pc_fail("setuid() is declared without a prototype");
	if (!IS_T(&(sleep), unsigned (*)(unsigned)))
		pc_fail("sleep() is not declared as unsigned sleep(unsigned)");
	if (IS_T(&(sleep), unsigned (*)(unsigned, int)))
		pc_fail("sleep() is declared without a prototype");
	if (!IS_T(&(symlink), int (*)(const char *, const char *)))
		pc_fail("symlink() is not declared as int symlink(const char *, const char *)");
	if (IS_T(&(symlink), int (*)(const char *, const char *, int)))
		pc_fail("symlink() is declared without a prototype");
	if (!IS_T(&(symlinkat), int (*)(const char *, int, const char *)))
		pc_fail("symlinkat() is not declared as int symlinkat(const char *, int, const char *)");
	if (IS_T(&(symlinkat), int (*)(const char *, int, const char *, int)))
		pc_fail("symlinkat() is declared without a prototype");
	if (!IS_T(&(sysconf), long (*)(int)))
		pc_fail("sysconf() is not declared as long sysconf(int)");
	if (IS_T(&(sysconf), long (*)(int, int)))
		pc_fail("sysconf() is declared without a prototype");
	if (!IS_T(&(tcgetpgrp), pid_t (*)(int)))
		pc_fail("tcgetpgrp() is not declared as pid_t tcgetpgrp(int)");
	if (IS_T(&(tcgetpgrp), pid_t (*)(int, int)))
		pc_fail("tcgetpgrp() is declared without a prototype");
	if (!IS_T(&(tcsetpgrp), int (*)(int, pid_t)))
		pc_fail("tcsetpgrp() is not declared as int tcsetpgrp(int, pid_t)");
	if (IS_T(&(tcsetpgrp), int (*)(int, pid_t, int)))
		pc_fail("tcsetpgrp() is declared without a prototype");
	if (!IS_T(&(truncate), int (*)(const char *, off_t)))
		pc_fail("truncate() is not declared as int truncate(const char *, off_t)");
	if (IS_T(&(truncate), int (*)(const char *, off_t, int)))
		pc_fail("truncate() is declared without a prototype");
	if (!IS_T(&(ttyname), char *(*)(int)))
		pc_fail("ttyname() is not declared as char *ttyname(int)");
	if (IS_T(&(ttyname), char *(*)(int, int)))
		pc_fail("ttyname() is declared without a prototype");
	if (!IS_T(&(ttyname_r), int (*)(int, char *, size_t)))
		pc_fail("ttyname_r() is not declared as int ttyname_r(int, char *, size_t)");
	if (IS_T(&(ttyname_r), int (*)(int, char *, size_t, int)))
		pc_fail("ttyname_r() is declared without a prototype");
	if (!IS_T(&(unlink), int (*)(const char *)))
		pc_fail("unlink() is not declared as int unlink(const char *)");
	if (IS_T(&(unlink), int (*)(const char *, int)))
		pc_fail("unlink() is declared without a prototype");
	if (!IS_T(&(unlinkat), int (*)(int, const char *, int)))
		pc_fail("unlinkat() is not declared as int unlinkat(int, const char *, int)");
	if (IS_T(&(unlinkat), int (*)(int, const char *, int, int)))
		pc_fail("unlinkat() is declared without a prototype");
	if (!IS_T(&(write), ssize_t (*)(int, const void *, size_t)))
		pc_fail("write() is not declared as ssize_t write(int, const void *, size_t)");
	if (IS_T(&(write), ssize_t (*)(int, const void *, size_t, int)))
		pc_fail("write() is declared without a prototype");
	pc_pass();
}
