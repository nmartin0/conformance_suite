# kill -number sends the signal for 0, 9 and 15.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.009
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.009
p= q=
trap 'kill -s KILL "$p" "$q" 2>/dev/null' EXIT
pc_test
sleep 100 &
p=$!
sleep 100 &
q=$!
kill -0 "$p" 2>/dev/null || pc_fail "kill -0 failed for a live process"
kill -9 "$p" 2>/dev/null || pc_fail "kill -9 failed"
kill -15 "$q" 2>/dev/null || pc_fail "kill -15 failed"
wait "$p"
n=$(kill -l "$?")
[ "$n" = KILL ] || pc_fail "after -9 the process ended by '$n'"
wait "$q"
n=$(kill -l "$?")
[ "$n" = TERM ] || pc_fail "after -15 the process ended by '$n'"
pc_pass
