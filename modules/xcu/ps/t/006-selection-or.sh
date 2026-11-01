# Selection criteria are ORed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ps.006
# MUTANT    ps.sel_and
# MUTANT    ps.sel_p_only
# MUTANT    ps.sel_u_only
# MUTANT    ps.sel_first
# MUTANT    ps.sel_last
# MUTANT    ps.p_ignored
# VARIANT   ps.reversed_lines
. "$PC_CORE/pc.sh"
pc_begin XCU.ps.006
LC_ALL=C; export LC_ALL
bg= fp=
trap '[ -n "$bg" ] && { kill "$bg" 2>/dev/null; wait "$bg" 2>/dev/null; }; [ -n "$fp" ] && { kill "$fp" 2>/dev/null; wait "$fp" 2>/dev/null; }' EXIT
sleep 301 </dev/null >/dev/null 2>&1 &
bg=$!
ru=$(id -ur) || pc_fail "id -ur failed"
# When the suite runs with privilege, a process of another user (real and
# effective user and group 65534) exists too, so that the two criteria select
# different sets.
if [ "$(id -u)" -eq 0 ]; then
	cat >asu.c <<'PC_EOF'
#include <unistd.h>
int main(int argc, char **argv)
{
	if (argc < 2) return 98;
	if (setgid(65534) != 0 || setuid(65534) != 0 || getuid() == 0) return 99;
	execvp(argv[1], argv + 1);
	return 127;
}
PC_EOF
	if ${PC_CC:-cc} -D_POSIX_C_SOURCE=202405L -o asu asu.c >/dev/null 2>&1; then
		./asu sleep 302 </dev/null >/dev/null 2>&1 &
		fp=$!
	fi
fi
# look: ps -o pid= ARGS...: fa fb ff say whether the shell, the child and the
# other user's process are listed.
look() {
	ps -o pid= "$@" >out 2>err </dev/null || pc_fail "ps $* failed"
	fa=no fb=no ff=no
	while IFS= read -r l; do
		set -- $l
		[ "$1" = "$$" ] && fa=yes
		[ "$1" = "$bg" ] && fb=yes
		[ -n "$fp" ] && [ "$1" = "$fp" ] && ff=yes
	done <out
}
pc_test
if [ -n "$fp" ]; then
	# wait until the other user's process has become that user's sleep
	i=0
	while :; do
		c=$(ps -o comm= -p "$fp" 2>/dev/null </dev/null)
		[ "$c" = sleep ] && break
		i=$((i + 1))
		[ "$i" -lt 10 ] || { fp=; break; }
		sleep 1
	done
fi
look -p "$bg" -U "$ru"
[ "$fb" = yes ] || pc_fail "the child $bg was not listed"
[ "$fa" = yes ] || pc_fail "the shell $$ (real user $ru) was not listed: criteria were not ORed"
look -U "$ru" -p "$bg"
[ "$fb" = yes ] || pc_fail "the child $bg was not listed (-U first)"
[ "$fa" = yes ] || pc_fail "the shell $$ was not listed (-U first)"
if [ -n "$fp" ]; then
	look -p "$fp" -U "$ru"
	[ "$ff" = yes ] || pc_fail "the process $fp of another user was not listed by -p $fp -U $ru"
	[ "$fa" = yes ] && [ "$fb" = yes ] || pc_fail "-p $fp -U $ru did not list the processes of real user $ru"
	look -U "$ru" -p "$fp"
	[ "$ff" = yes ] || pc_fail "the process $fp of another user was not listed by -U $ru -p $fp"
	[ "$fa" = yes ] && [ "$fb" = yes ] || pc_fail "-U $ru -p $fp did not list the processes of real user $ru"
fi
pc_pass
