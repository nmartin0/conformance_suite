# kill can be run by exec.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.010
# MUTANT    kill.exec_drops
# MUTANT    kill.exec_exit1
# MUTANT    kill.exec_stdout
# MUTANT    kill.exec_zero_exit1
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.010
p= q=
trap 'command kill -s KILL "$p" "$q" 2>/dev/null' EXIT
pc_test
sleep 30 &
p=$!
sleep 30 &
q=$!
env kill -s 0 "$q" 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "env kill -s 0 exited $rc"
env kill -s KILL "$p" >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "env kill -s KILL exited $rc"
[ ! -s out ] || pc_fail "env kill -s KILL wrote to standard output"
wait "$p"
n=$(command kill -l "$?")
[ "$n" = KILL ] || pc_fail "the process ended by '$n' after env kill -s KILL"
env kill -s KILL "$q" 2>/dev/null || pc_fail "env kill -s KILL failed on the second process"
wait "$q"
n=$(command kill -l "$?")
[ "$n" = KILL ] || pc_fail "the second process ended by '$n' after env kill -s KILL"
pc_pass
