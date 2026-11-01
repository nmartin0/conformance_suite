# -A, -U and -G select the test shell.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ps.007
# MUTANT    ps.A_default
# MUTANT    ps.U_all
# MUTANT    ps.G_all
# MUTANT    ps.U_ignored
# MUTANT    ps.G_ignored
# MUTANT    ps.U_first_only
# MUTANT    ps.U_last_only
# MUTANT    ps.G_first_only
# MUTANT    ps.G_last_only
# MUTANT    ps.U_name_rejected
# MUTANT    ps.U_name_nobody
# MUTANT    ps.exit1
# VARIANT   ps.reversed_lines
. "$PC_CORE/pc.sh"
pc_begin XCU.ps.007
LC_ALL=C; export LC_ALL
bg= fp=
trap '[ -n "$bg" ] && { kill "$bg" 2>/dev/null; wait "$bg" 2>/dev/null; }; [ -n "$fp" ] && { kill "$fp" 2>/dev/null; wait "$fp" 2>/dev/null; }' EXIT
sleep 301 </dev/null >/dev/null 2>&1 &
bg=$!
ru=$(id -ur) || pc_fail "id -ur failed"
rg=$(id -gr) || pc_fail "id -gr failed"
run=$(id -urn 2>/dev/null)
# When the suite runs with privilege, a process of another user (real and
# effective user and group 65534) exists too: selection by user and group can
# then be told from the default selection and from "all".
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
# listed OPTION-ARGS...: run ps, set fa fb ff to whether $$, $bg and the other
# user's process appear.
listed() {
	ps -o pid= "$@" >out 2>err </dev/null || pc_fail "ps $* failed"
	fa=no fb=no ff=no
	while IFS= read -r l; do
		set -- $l
		[ "$1" = "$$" ] && fa=yes
		[ "$1" = "$bg" ] && fb=yes
		[ -n "$fp" ] && [ "$1" = "$fp" ] && ff=yes
	done <out
}
# wait until the other process is that user's sleep (setup phase: ps is the real one)
if [ -n "$fp" ]; then
	i=0
	while :; do
		c=$(ps -o comm= -p "$fp" 2>/dev/null </dev/null)
		[ "$c" = sleep ] && break
		i=$((i + 1))
		[ "$i" -lt 10 ] || { fp=; break; }
		sleep 1
	done
fi
pc_test
listed -A
[ "$fa" = yes ] && [ "$fb" = yes ] || pc_fail "ps -A did not list both $$ and $bg"
[ -z "$fp" ] || [ "$ff" = yes ] || pc_fail "ps -A did not list $fp, a process of another user"
listed -U "$ru"
[ "$fa" = yes ] || pc_fail "ps -U $ru did not list $$"
[ "$ff" = no ] || pc_fail "ps -U $ru listed $fp, a process of another real user"
listed -G "$rg"
[ "$fa" = yes ] || pc_fail "ps -G $rg did not list $$"
[ "$ff" = no ] || pc_fail "ps -G $rg listed $fp, a process of another real group"
if [ -n "$run" ]; then
	listed -U "$run"
	[ "$fa" = yes ] || pc_fail "ps -U $run (a login name) did not list $$"
fi
if [ -n "$fp" ]; then
	listed -U 65534
	[ "$ff" = yes ] || pc_fail "ps -U 65534 did not list $fp"
	[ "$fa" = no ] || pc_fail "ps -U 65534 listed $$, whose real user is $ru"
	listed -G 65534
	[ "$ff" = yes ] || pc_fail "ps -G 65534 did not list $fp"
	[ "$fa" = no ] || pc_fail "ps -G 65534 listed $$, whose real group is $rg"
	for sep in , ' '; do
		listed -U "$ru${sep}65534"
		[ "$fa" = yes ] && [ "$ff" = yes ] || pc_fail "ps -U '$ru${sep}65534' did not list both users' processes"
		listed -U "65534${sep}$ru"
		[ "$fa" = yes ] && [ "$ff" = yes ] || pc_fail "ps -U '65534${sep}$ru' did not list both users' processes"
		listed -G "$rg${sep}65534"
		[ "$fa" = yes ] && [ "$ff" = yes ] || pc_fail "ps -G '$rg${sep}65534' did not list both groups' processes"
		listed -G "65534${sep}$rg"
		[ "$fa" = yes ] && [ "$ff" = yes ] || pc_fail "ps -G '65534${sep}$rg' did not list both groups' processes"
	done
fi
pc_pass
