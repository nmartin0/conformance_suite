# XSI options -e, -u, -f, -l.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ps.008
# MUTANT    ps.e_default
# MUTANT    ps.u_all
# MUTANT    ps.u_ignored
# MUTANT    ps.u_name_rejected
# MUTANT    ps.u_first_only
# MUTANT    ps.f_ignored
# MUTANT    ps.l_ignored
# MUTANT    ps.f_swap_pids
# MUTANT    ps.f_drop_stime
# MUTANT    ps.f_drop_cmd
# MUTANT    ps.f_cmd_first
# MUTANT    ps.f_uid_numeric
# MUTANT    ps.l_swap_pids
# MUTANT    ps.l_drop_wchan
# MUTANT    ps.l_drop_sz
# MUTANT    ps.l_drop_flags
# MUTANT    ps.l_uid_name
# VARIANT   ps.reversed_lines
. "$PC_CORE/pc.sh"
pc_begin XCU.ps.008
LC_ALL=C; export LC_ALL
eu=$(id -u) || pc_fail "id -u failed"
eun=$(id -un 2>/dev/null)
fp=
trap '[ -n "$fp" ] && { kill "$fp" 2>/dev/null; wait "$fp" 2>/dev/null; }' EXIT
# When the suite runs with privilege, a process of another user (real and
# effective user and group 65534) exists too: -e and -u can then be told from
# the default selection.
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
# inorder WORD...: the words of $l appear in this order (others may lie between).
inorder() {
	set -- "$@"
	rest=" $l "
	for w in "$@"; do
		case $rest in
		*" $w "*) rest=" ${rest#*" $w "}" ;;
		*) return 1 ;;
		esac
	done
	return 0
}
# colword HEADING: the word of the second line of out that lies under the
# heading word of the first line (both lines split into words).
colword() {
	read -r hl <out
	dl=$(sed -n 2p out)
	k=0 pos=0
	for w in $hl; do
		k=$((k + 1))
		[ "$w" = "$1" ] && { pos=$k; break; }
	done
	[ "$pos" -gt 0 ] || return 1
	set -- $dl
	[ "$#" -ge "$pos" ] || return 1
	eval "cw=\${$pos}"
}
# listed OPTION-ARGS...: run ps, set fa ff to whether $$ and the other user's
# process appear.
listed() {
	ps -o pid= "$@" >out 2>err </dev/null || pc_fail "ps $* failed"
	fa=no ff=no
	while IFS= read -r l; do
		set -- $l
		[ "$1" = "$$" ] && fa=yes
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
listed -e
[ "$fa" = yes ] || pc_fail "ps -e did not list $$"
[ -z "$fp" ] || [ "$ff" = yes ] || pc_fail "ps -e did not list $fp, a process of another user"
listed -u "$eu"
[ "$fa" = yes ] || pc_fail "ps -u $eu did not list $$"
[ "$ff" = no ] || pc_fail "ps -u $eu listed $fp, a process of another user"
if [ -n "$eun" ]; then
	listed -u "$eun"
	[ "$fa" = yes ] || pc_fail "ps -u $eun (a login name) did not list $$"
fi
if [ -n "$fp" ]; then
	listed -u 65534
	[ "$ff" = yes ] || pc_fail "ps -u 65534 did not list $fp"
	[ "$fa" = no ] || pc_fail "ps -u 65534 listed $$, whose effective user is $eu"
	listed -u "$eu,65534"
	[ "$fa" = yes ] && [ "$ff" = yes ] || pc_fail "ps -u '$eu,65534' did not list both users' processes"
	listed -u "65534 $eu"
	[ "$fa" = yes ] && [ "$ff" = yes ] || pc_fail "ps -u '65534 $eu' did not list both users' processes"
fi
ps -f -p "$$" >out 2>err </dev/null || pc_fail "ps -f failed"
read -r l <out
inorder UID PID PPID C STIME TTY TIME CMD || pc_fail "ps -f headings are '$l'"
if [ -n "$eun" ]; then
	colword UID || pc_fail "ps -f wrote no UID column for $$"
	[ "$cw" = "$eun" ] || pc_fail "ps -f shows '$cw' under UID, expected the login name $eun"
fi
colword PID || pc_fail "ps -f wrote no PID column for $$"
[ "$cw" = "$$" ] || pc_fail "ps -f shows '$cw' under PID, expected $$"
colword PPID || pc_fail "ps -f wrote no PPID column for $$"
[ "$cw" = "$PPID" ] || pc_fail "ps -f shows '$cw' under PPID, expected $PPID"
ps -l -p "$$" >out 2>err </dev/null || pc_fail "ps -l failed"
read -r l <out
inorder F S UID PID PPID C PRI NI ADDR SZ WCHAN TTY TIME CMD || pc_fail "ps -l headings are '$l'"
colword UID || pc_fail "ps -l wrote no UID column for $$"
[ "$cw" = "$eu" ] || pc_fail "ps -l shows '$cw' under UID, expected the number $eu"
pc_pass
