# kill -l with a wait status names the signal.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.006
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.006
p= q=
trap 'kill -s KILL "$p" "$q" 2>/dev/null' EXIT
nl='
'
pc_test
sleep 100 &
p=$!
kill -s KILL "$p" || pc_fail "kill failed"
wait "$p"
st=$?
kill -l "$st" >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "kill -l $st exited $rc"
got=$(cat out; echo x)
[ "$got" = "KILL${nl}x" ] || pc_fail "kill -l $st wrote something other than KILL and a newline"
sleep 100 &
q=$!
sleep 1
kill -s TERM "$q" || pc_fail "kill failed"
wait "$q"
st=$?
got=$(kill -l "$st")
[ "$got" = TERM ] || pc_fail "kill -l $st wrote '$got', wanted TERM"
pc_pass
