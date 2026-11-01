# ps with no options on a terminal lists the invoking shell and its children
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ps.014
# MUTANT    ps.d_empty
# MUTANT    ps.d_all
# MUTANT    ps.d_user_only
# MUTANT    ps.d_tty_only
# MUTANT    ps.d_no_shell
# MUTANT    ps.d_no_children
# MUTANT    ps.exit1
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.ps.014
LC_ALL=C; export LC_ALL
pc_pty_need
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
out=
trap '[ -n "$out" ] && { kill "$out" 2>/dev/null; wait "$out" 2>/dev/null; }; cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# a process of the same user with no controlling terminal: not selected
sleep 61 </dev/null >/dev/null 2>&1 &
out=$!
# When the suite runs with privilege, the shell on the terminal also starts a
# process of another user (65534) on that terminal: not selected either.
asu=
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
	${PC_CC:-cc} -D_POSIX_C_SOURCE=202405L -o asu asu.c >/dev/null 2>&1 && asu=$PWD/asu
fi
{
	echo "asu='$asu'"
	cat <<'IN'
sleep 60 </dev/null >/dev/null 2>&1 &
b=$!
echo $b >bg.pid
echo $$ >sh.pid
if [ -n "$asu" ]; then
	"$asu" sleep 62 </dev/null >/dev/null 2>&1 &
	f=$!
	i=0
	while [ "$i" -lt 10 ]; do
		c=$(ps -o comm= -p "$f" 2>/dev/null)
		[ "$c" = sleep ] && break
		sleep 1
		i=$((i + 1))
	done
	echo $f >fo.pid
fi
ps -o pid= >ps.out 2>ps.err
echo $? >ps.rc
kill $b $f 2>/dev/null
wait $b 2>/dev/null
exit 0
IN
} >inner.sh
: >typed
pc_test
pc_pty_run -c -i typed inner.sh
[ "$(cat ps.rc 2>/dev/null)" = 0 ] || pc_fail "ps: exit status '$(cat ps.rc 2>/dev/null)'"
b=$(cat bg.pid 2>/dev/null)
s=$(cat sh.pid 2>/dev/null)
f=$(cat fo.pid 2>/dev/null)
[ -n "$b" ] && [ -n "$s" ] || pc_unresolved "the processes were not started"
fb=; fs=; ff=; fo=
while read -r l; do
	for w in $l; do
		[ "$w" = "$b" ] && fb=1
		[ "$w" = "$s" ] && fs=1
		[ "$w" = "$out" ] && fo=1
		[ -n "$f" ] && [ "$w" = "$f" ] && ff=1
	done
done <ps.out
[ -n "$fs" ] || pc_fail "ps did not list the invoking shell $s: $(cat ps.out)"
[ -n "$fb" ] || pc_fail "ps did not list the background child $b: $(cat ps.out)"
[ -z "$fo" ] || pc_fail "ps listed process $out, which has no controlling terminal: $(cat ps.out)"
[ -z "$ff" ] || pc_fail "ps listed process $f, which has another effective user: $(cat ps.out)"
pc_pass
