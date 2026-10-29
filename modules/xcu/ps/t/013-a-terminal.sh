# ps -a lists a non-leader process attached to the terminal
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ps.013
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.ps.013
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
ps -a -o pid= >ps.out 2>ps.err
echo $? >ps.rc
kill $b 2>/dev/null
wait $b 2>/dev/null
exit 0
IN
: >typed
pc_test
pc_pty_run -c -i typed inner.sh
[ "$(cat ps.rc 2>/dev/null)" = 0 ] || pc_fail "ps -a: exit status '$(cat ps.rc 2>/dev/null)'"
b=$(cat bg.pid 2>/dev/null)
[ -n "$b" ] || pc_unresolved "the background process was not started"
found=
while read -r p; do [ "$p" = "$b" ] && found=1; done <ps.out
[ -n "$found" ] || pc_fail "ps -a did not list process $b, which is attached to the terminal"
pc_pass
