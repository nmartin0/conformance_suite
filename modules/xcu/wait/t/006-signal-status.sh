# A process ended by a signal gives a status above 128 per signal.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wait.006
. "$PC_CORE/pc.sh"
pc_begin XCU.wait.006
p= q=
trap 'kill -s KILL "$p" "$q" 2>/dev/null' EXIT
pc_test
sleep 100 &
p=$!
kill -s KILL "$p"
wait "$p"
st1=$?
p=
sleep 100 &
q=$!
kill -s TERM "$q"
wait "$q"
st2=$?
q=
[ "$st1" -gt 128 ] || pc_fail "status $st1 for a process killed by SIGKILL"
[ "$st2" -gt 128 ] || pc_fail "status $st2 for a process ended by SIGTERM"
[ "$st1" -ne "$st2" ] || pc_fail "the same status $st1 for SIGKILL and SIGTERM"
n1=$(kill -l "$st1")
n2=$(kill -l "$st2")
[ "$n1" = KILL ] || pc_fail "kill -l $st1 gives '$n1', wanted KILL"
[ "$n2" = TERM ] || pc_fail "kill -l $st2 gives '$n2', wanted TERM"
pc_pass
