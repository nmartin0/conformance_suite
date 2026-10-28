/*
 * Every required header, in alphabetical order, each twice in a row: the program builds and works.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.2-compilation-environment.002
 */
#include <aio.h>
#include <aio.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <assert.h>
#include <assert.h>
#include <cpio.h>
#include <cpio.h>
#include <ctype.h>
#include <ctype.h>
#include <dirent.h>
#include <dirent.h>
#include <dlfcn.h>
#include <dlfcn.h>
#include <endian.h>
#include <endian.h>
#include <errno.h>
#include <errno.h>
#include <fcntl.h>
#include <fcntl.h>
#include <fenv.h>
#include <fenv.h>
#include <float.h>
#include <float.h>
#include <fnmatch.h>
#include <fnmatch.h>
#include <glob.h>
#include <glob.h>
#include <grp.h>
#include <grp.h>
#include <iconv.h>
#include <iconv.h>
#include <inttypes.h>
#include <inttypes.h>
#include <iso646.h>
#include <iso646.h>
#include <langinfo.h>
#include <langinfo.h>
#include <libintl.h>
#include <libintl.h>
#include <limits.h>
#include <limits.h>
#include <locale.h>
#include <locale.h>
#include <math.h>
#include <math.h>
#include <monetary.h>
#include <monetary.h>
#include <net/if.h>
#include <net/if.h>
#include <netdb.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/tcp.h>
#include <nl_types.h>
#include <nl_types.h>
#include <poll.h>
#include <poll.h>
#include <pthread.h>
#include <pthread.h>
#include <pwd.h>
#include <pwd.h>
#include <regex.h>
#include <regex.h>
#include <sched.h>
#include <sched.h>
#include <semaphore.h>
#include <semaphore.h>
#include <setjmp.h>
#include <setjmp.h>
#include <signal.h>
#include <signal.h>
#include <stdalign.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdbool.h>
#include <stddef.h>
#include <stddef.h>
#include <stdint.h>
#include <stdint.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <stdnoreturn.h>
#include <string.h>
#include <string.h>
#include <strings.h>
#include <strings.h>
#include <sys/mman.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/resource.h>
#include <sys/select.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/statvfs.h>
#include <sys/times.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/un.h>
#include <sys/utsname.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <sys/wait.h>
#include <tar.h>
#include <tar.h>
#include <termios.h>
#include <termios.h>
#include <time.h>
#include <time.h>
#include <uchar.h>
#include <uchar.h>
#include <unistd.h>
#include <unistd.h>
#include <wchar.h>
#include <wchar.h>
#include <wctype.h>
#include <wctype.h>
#include <wordexp.h>
#include <wordexp.h>
#include "pc.h"

int main(void)
{
	struct stat st;
	DIR *d;
	pthread_t self;
	regex_t re;
	sigset_t ss;
	struct sockaddr_in sin;
	struct pollfd pfd;
	struct timespec ts;
	wchar_t wc = L'x';
	size_t n = sizeof sin + sizeof pfd + sizeof re;

	pc_begin("XSH.2.2-compilation-environment.002");
	pc_test();
	pc_check(stat(".", &st) == 0 && S_ISDIR(st.st_mode), "stat(\".\") did not give a directory");
	d = opendir(".");
	pc_check(d != NULL, "opendir failed");
	(void)closedir(d);
	self = pthread_self();
	pc_check(pthread_equal(self, self) != 0, "pthread_equal(self, self) is false");
	pc_check(sigemptyset(&ss) == 0 && sigaddset(&ss, SIGINT) == 0 && sigismember(&ss, SIGINT) == 1,
	    "the signal set functions failed");
	pc_check(clock_gettime(CLOCK_MONOTONIC, &ts) == 0, "clock_gettime failed");
	pc_check(wc == L'x' && n > 0 && CHAR_BIT == 8 && EOF < 0 && EINVAL > 0, "basic constants are wrong");
	pc_check(O_RDONLY == 0 || O_RDONLY != O_WRONLY, "O_RDONLY and O_WRONLY are the same");
	assert(n > 0);
	pc_pass();
}
