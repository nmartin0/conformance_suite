/*
 * After every required header, names no header may define are free for the program.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.2-compilation-environment.004
 */
#include <aio.h>
#include <arpa/inet.h>
#include <assert.h>
#include <cpio.h>
#include <ctype.h>
#include <dirent.h>
#include <dlfcn.h>
#include <endian.h>
#include <errno.h>
#include <fcntl.h>
#include <fenv.h>
#include <float.h>
#include <fnmatch.h>
#include <glob.h>
#include <grp.h>
#include <iconv.h>
#include <inttypes.h>
#include <iso646.h>
#include <langinfo.h>
#include <libintl.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <monetary.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <nl_types.h>
#include <poll.h>
#include <pthread.h>
#include <pwd.h>
#include <regex.h>
#include <sched.h>
#include <semaphore.h>
#include <setjmp.h>
#include <signal.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <strings.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <tar.h>
#include <termios.h>
#include <time.h>
#include <uchar.h>
#include <unistd.h>
#include <wchar.h>
#include <wctype.h>
#include <wordexp.h>
#include "pc.h"

#ifdef MIN
#error "MIN is defined as a macro by a header"
#endif
#ifdef MAX
#error "MAX is defined as a macro by a header"
#endif
#ifdef TRUE
#error "TRUE is defined as a macro by a header"
#endif
#ifdef FALSE
#error "FALSE is defined as a macro by a header"
#endif
#ifdef MAXPATHLEN
#error "MAXPATHLEN is defined as a macro by a header"
#endif
#ifdef NBBY
#error "NBBY is defined as a macro by a header"
#endif
#ifdef howmany
#error "howmany is defined as a macro by a header"
#endif
#ifdef roundup
#error "roundup is defined as a macro by a header"
#endif
#ifdef powerof2
#error "powerof2 is defined as a macro by a header"
#endif

static int index = 1;
static int rindex = 1;
static int bcopy = 1;
static int bzero = 1;
static int bcmp = 1;
static int valloc = 1;
static int getdtablesize = 1;
static int getpagesize = 1;
static int ulong = 1;
static int ushort = 1;
static int uint = 1;
static int u_char = 1;
static int u_short = 1;
static int u_int = 1;
static int u_long = 1;
static int min = 1;
static int max = 1;

int main(void)
{
	int sum = 0;

	pc_begin("XSH.2.2-compilation-environment.004");
	pc_test();
	sum += index;
	sum += rindex;
	sum += bcopy;
	sum += bzero;
	sum += bcmp;
	sum += valloc;
	sum += getdtablesize;
	sum += getpagesize;
	sum += ulong;
	sum += ushort;
	sum += uint;
	sum += u_char;
	sum += u_short;
	sum += u_int;
	sum += u_long;
	sum += min;
	sum += max;
	pc_check(sum == 17, "the program's own objects were changed");
	pc_pass();
}
