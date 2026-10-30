# kill pid sends SIGTERM by default.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.001
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.001
p=
trap 'kill -s KILL "$p" 2>/dev/null' EXIT
pc_test
sleep 100 &
p=$!
sleep 1
kill "$p" >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "kill exited $rc"
[ ! -s out ] || pc_fail "kill wrote to standard output"
wait "$p"
st=$?
[ "$st" -gt 128 ] || pc_fail "the process did not end by a signal (status $st)"
n=$(kill -l "$st")
[ "$n" = TERM ] || pc_fail "the process ended by '$n', wanted TERM"
# the same through a direct exec of the utility (a shell's own kill may be
# an intrinsic that never reaches the utility)
sleep 100 &
p=$!
sleep 1
env kill "$p" >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "env kill exited $rc"
[ ! -s out ] || pc_fail "env kill wrote to standard output"
wait "$p"
st=$?
[ "$st" -gt 128 ] || pc_fail "after env kill the process did not end by a signal (status $st)"
n=$(kill -l "$st")
[ "$n" = TERM ] || pc_fail "after env kill the process ended by '$n', wanted TERM"
pc_pass
