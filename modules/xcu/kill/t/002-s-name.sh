# kill -s takes a name in any case, without SIG.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.002
# MUTANT    kill.s_ignored
# MUTANT    kill.s_lower_rejected
# MUTANT    kill.s_upper_rejected
# MUTANT    kill.s_mixed_rejected
# MUTANT    kill.s_needs_prefix
# MUTANT    kill.s_usr1_as_usr2
# MUTANT    kill.send_stdout
# MUTANT    kill.send_status1
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.002
p= q= r=
trap 'command kill -s KILL "$p" "$q" "$r" 2>/dev/null' EXIT
pc_test
sleep 30 &
p=$!
sleep 30 &
q=$!
sleep 30 &
r=$!
sleep 1
: >out
kill -s term "$p" >>out 2>/dev/null || pc_fail "kill -s term failed"
kill -s KILL "$q" >>out 2>/dev/null || pc_fail "kill -s KILL failed"
kill -s Usr1 "$r" >>out 2>/dev/null || pc_fail "kill -s Usr1 failed"
[ ! -s out ] || pc_fail "kill wrote to standard output"
wait "$p"
n=$(command kill -l "$?")
[ "$n" = TERM ] || pc_fail "after -s term the process ended by '$n'"
wait "$q"
n=$(command kill -l "$?")
[ "$n" = KILL ] || pc_fail "after -s KILL the process ended by '$n'"
wait "$r"
n=$(command kill -l "$?")
[ "$n" = USR1 ] || pc_fail "after -s Usr1 the process ended by '$n'"
# the same through a direct exec of the utility
sleep 30 &
p=$!
sleep 30 &
q=$!
sleep 30 &
r=$!
sleep 1
: >out
env kill -s term "$p" >>out 2>/dev/null || pc_fail "env kill -s term failed"
env kill -s KILL "$q" >>out 2>/dev/null || pc_fail "env kill -s KILL failed"
env kill -s Usr1 "$r" >>out 2>/dev/null || pc_fail "env kill -s Usr1 failed"
[ ! -s out ] || pc_fail "kill wrote to standard output"
wait "$p"
n=$(command kill -l "$?")
[ "$n" = TERM ] || pc_fail "after env kill -s term the process ended by '$n'"
wait "$q"
n=$(command kill -l "$?")
[ "$n" = KILL ] || pc_fail "after env kill -s KILL the process ended by '$n'"
wait "$r"
n=$(command kill -l "$?")
[ "$n" = USR1 ] || pc_fail "after env kill -s Usr1 the process ended by '$n'"
pc_pass
