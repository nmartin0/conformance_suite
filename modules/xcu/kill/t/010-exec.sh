# kill can be run by exec.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.010
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.010
p=
trap 'kill -s KILL "$p" 2>/dev/null' EXIT
pc_test
env kill -s 0 $$ 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "env kill -s 0 exited $rc"
sleep 100 &
p=$!
env kill -s KILL "$p" 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "env kill -s KILL exited $rc"
wait "$p"
n=$(kill -l "$?")
[ "$n" = KILL ] || pc_fail "the process ended by '$n' after env kill -s KILL"
pc_pass
