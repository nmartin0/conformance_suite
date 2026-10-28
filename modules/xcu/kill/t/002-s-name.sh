# kill -s takes a name in any case, without SIG.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.002
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.002
p= q= r=
trap 'kill -s KILL "$p" "$q" "$r" 2>/dev/null' EXIT
pc_test
sleep 100 &
p=$!
sleep 100 &
q=$!
sleep 100 &
r=$!
kill -s term "$p" 2>/dev/null || pc_fail "kill -s term failed"
kill -s KILL "$q" 2>/dev/null || pc_fail "kill -s KILL failed"
kill -s Usr1 "$r" 2>/dev/null || pc_fail "kill -s Usr1 failed"
wait "$p"
n=$(kill -l "$?")
[ "$n" = TERM ] || pc_fail "after -s term the process ended by '$n'"
wait "$q"
n=$(kill -l "$?")
[ "$n" = KILL ] || pc_fail "after -s KILL the process ended by '$n'"
wait "$r"
n=$(kill -l "$?")
[ "$n" = USR1 ] || pc_fail "after -s Usr1 the process ended by '$n'"
pc_pass
