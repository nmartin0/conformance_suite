# kill -NAME works like kill -s NAME.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.008
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.008
p= q=
trap 'kill -s KILL "$p" "$q" 2>/dev/null' EXIT
pc_test
sleep 100 &
p=$!
sleep 100 &
q=$!
kill -KILL "$p" 2>/dev/null || pc_fail "kill -KILL failed"
kill -TERM "$q" 2>/dev/null || pc_fail "kill -TERM failed"
wait "$p"
n=$(kill -l "$?")
[ "$n" = KILL ] || pc_fail "after -KILL the process ended by '$n'"
wait "$q"
n=$(kill -l "$?")
[ "$n" = TERM ] || pc_fail "after -TERM the process ended by '$n'"
# the same through a direct exec of the utility
sleep 100 &
p=$!
sleep 100 &
q=$!
env kill -KILL "$p" 2>/dev/null || pc_fail "env kill -KILL failed"
env kill -TERM "$q" 2>/dev/null || pc_fail "env kill -TERM failed"
wait "$p"
n=$(kill -l "$?")
[ "$n" = KILL ] || pc_fail "after env kill -KILL the process ended by '$n'"
wait "$q"
n=$(kill -l "$?")
[ "$n" = TERM ] || pc_fail "after env kill -TERM the process ended by '$n'"
pc_pass
