# ps with no options on a terminal lists the invoking shell and its children
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ps.014
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.ps.014
LC_ALL=C; export LC_ALL
pc_pty_need
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >inner.sh <<'IN'
sleep 60 </dev/null >/dev/null 2>&1 &
b=$!
echo $b >bg.pid
echo $$ >sh.pid
ps -o pid= >ps.out 2>ps.err
echo $? >ps.rc
kill $b 2>/dev/null
wait $b 2>/dev/null
exit 0
IN
: >typed
pc_test
pc_pty_run -i typed inner.sh
[ "$(cat ps.rc 2>/dev/null)" = 0 ] || pc_fail "ps: exit status '$(cat ps.rc 2>/dev/null)'"
b=$(cat bg.pid 2>/dev/null)
s=$(cat sh.pid 2>/dev/null)
[ -n "$b" ] && [ -n "$s" ] || pc_unresolved "the processes were not started"
fb=; fs=
while read -r l; do
	for w in $l; do
		[ "$w" = "$b" ] && fb=1
		[ "$w" = "$s" ] && fs=1
	done
done <ps.out
[ -n "$fs" ] || pc_fail "ps did not list the invoking shell $s: $(cat ps.out)"
[ -n "$fb" ] || pc_fail "ps did not list the background child $b: $(cat ps.out)"
pc_pass
