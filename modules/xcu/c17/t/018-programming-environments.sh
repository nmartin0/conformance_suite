# getconf programming environments
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.c17.018
# The default and each supported named environment have the type sizes of the
# table, the width-restricted environments are listed and their flags work with
# c17, and the threaded environment flags can be obtained and used.
. "$PC_CORE/pc.sh"
pc_begin XCU.c17.018
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v getconf >/dev/null 2>&1 || pc_unresolved "setup: getconf is not available"
cat >sz.c <<'EOY'
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <termios.h>
#include <poll.h>
#include <wchar.h>
int main(void)
{
	size_t l = sizeof(long);
	int ok = sizeof(blksize_t) <= l && sizeof(cc_t) <= l && sizeof(mode_t) <= l &&
		sizeof(nfds_t) <= l && sizeof(pid_t) <= l && sizeof(ptrdiff_t) <= l &&
		sizeof(size_t) <= l && sizeof(speed_t) <= l && sizeof(ssize_t) <= l &&
		sizeof(suseconds_t) <= l && sizeof(tcflag_t) <= l &&
		sizeof(wchar_t) <= l && sizeof(wint_t) <= l;

	printf("%d %d %d %d %d\n", (int)(8 * sizeof(int)), (int)(8 * sizeof(long)),
		(int)(8 * sizeof(void *)), (int)(8 * sizeof(off_t)), ok);
	return 0;
}
EOY
cat >th.c <<'EOY'
#include <pthread.h>
static void *run(void *a) { return a; }
int main(void)
{
	pthread_t t;

	if (pthread_create(&t, 0, run, 0) != 0)
		return 1;
	return pthread_join(t, 0) != 0;
}
EOY
# row NAME SIZES: does "int long ptr off" match the table row of NAME
match() {
	echo "$2" | awk -v n="$1" '{
		if (n == "_POSIX_V8_ILP32_OFF32") r = ($1 == 32 && $2 == 32 && $3 == 32 && $4 == 32)
		else if (n == "_POSIX_V8_ILP32_OFFBIG") r = ($1 == 32 && $2 == 32 && $3 == 32 && $4 >= 64)
		else if (n == "_POSIX_V8_LP64_OFF64") r = ($1 == 32 && $2 == 64 && $3 == 64 && $4 == 64)
		else if (n == "_POSIX_V8_LPBIG_OFFBIG") r = ($1 >= 32 && $2 >= 64 && $3 >= 64 && $4 >= 64)
		else r = 0
		exit !r }'
}
pc_test
c17 -o pdef sz.c </dev/null >out 2>err || pc_fail "the default environment does not compile the size program"
def=$(./pdef </dev/null) || pc_fail "the default program does not run"
set -- $def
[ "$5" = 1 ] || pc_fail "the default environment has a standard type wider than long"
nsup=0
defmatch=0
for e in _POSIX_V8_ILP32_OFF32 _POSIX_V8_ILP32_OFFBIG _POSIX_V8_LP64_OFF64 _POSIX_V8_LPBIG_OFFBIG; do
	v=$(getconf "$e" 2>/dev/null) || continue
	case $v in -1|undefined|'') continue ;; esac
	nsup=$((nsup+1))
	match "$e" "$def" && defmatch=1
	b=${e#_}
	cf=$(getconf "${b}_CFLAGS" 2>/dev/null) || pc_fail "getconf ${b}_CFLAGS failed although $e is supported"
	ld=$(getconf "${b}_LDFLAGS" 2>/dev/null) || pc_fail "getconf ${b}_LDFLAGS failed"
	lb=$(getconf "${b}_LIBS" 2>/dev/null) || pc_fail "getconf ${b}_LIBS failed"
	c17 $cf -o pe $ld sz.c $lb </dev/null >out 2>err || pc_fail "environment $e: compiling with its flags failed"
	r=$(./pe </dev/null) || pc_fail "environment $e: the program does not run"
	match "$e" "$r" || pc_fail "environment $e has type sizes '$r' that do not fit its table row"
done
[ "$nsup" -ge 1 ] || pc_fail "none of the four named environments is reported as supported"
[ "$defmatch" -eq 1 ] || pc_fail "the default environment ($def) matches no supported named environment"
w=$(getconf POSIX_V8_WIDTH_RESTRICTED_ENVS 2>/dev/null) || pc_fail "getconf POSIX_V8_WIDTH_RESTRICTED_ENVS failed"
[ -n "$w" ] || pc_fail "no width-restricted environment is listed"
for e in $w; do
	cf=$(getconf "${e}_CFLAGS" 2>/dev/null) || pc_fail "getconf ${e}_CFLAGS failed"
	ld=$(getconf "${e}_LDFLAGS" 2>/dev/null) || pc_fail "getconf ${e}_LDFLAGS failed"
	lb=$(getconf "${e}_LIBS" 2>/dev/null) || pc_fail "getconf ${e}_LIBS failed"
	c17 $cf -o pw $ld sz.c $lb </dev/null >out 2>err || pc_fail "environment $e: compiling with its flags failed"
	r=$(./pw </dev/null) || pc_fail "environment $e: the program does not run"
	set -- $r
	[ "$5" = 1 ] || pc_fail "width-restricted environment $e has a standard type wider than long"
done
cf=$(getconf POSIX_V8_THREADS_CFLAGS 2>/dev/null) || pc_fail "getconf POSIX_V8_THREADS_CFLAGS failed"
ld=$(getconf POSIX_V8_THREADS_LDFLAGS 2>/dev/null) || pc_fail "getconf POSIX_V8_THREADS_LDFLAGS failed"
c17 $cf -o pt $ld th.c </dev/null >out 2>err || pc_fail "the threaded environment flags do not build a thread program"
./pt </dev/null || pc_fail "the thread program built with the threaded environment flags failed"
pc_pass
