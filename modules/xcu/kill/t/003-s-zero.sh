# kill -s 0 sends nothing and tests the process.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.003
# MUTANT    kill.zero_sends_term
# MUTANT    kill.zero_rejected
# MUTANT    kill.zero_gone_ok
# MUTANT    kill.gone_ok
# MUTANT    kill.gone_stdout
# VARIANT   kill.err_status77
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.003
p= s=
trap 'command kill -s KILL "$p" "$s" 2>/dev/null' EXIT
"${PC_SH:-sh}" -c 'exit 0' &
d=$!
wait "$d"
pc_test
sleep 30 &
p=$!
sleep 30 &
s=$!
kill -s 0 "$p" 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "kill -s 0 exited $rc for a live process"
kill -s KILL "$p" 2>/dev/null || pc_fail "kill -s KILL failed"
wait "$p"
n=$(command kill -l "$?")
[ "$n" = KILL ] || pc_fail "the process was ended by '$n': signal 0 may have been delivered as a real signal"
kill -s 0 "$d" >out 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "kill -s 0 exited $rc for a process that is gone"
[ ! -s out ] || pc_fail "kill -s 0 wrote to standard output"
# the same through a direct exec of the utility (a shell's own kill may
# be an intrinsic that never reaches the utility)
env kill -s 0 "$s" 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "env kill -s 0 exited $rc for a live process"
env kill -s KILL "$s" 2>/dev/null || pc_fail "env kill -s KILL failed"
wait "$s"
n=$(command kill -l "$?")
[ "$n" = KILL ] || pc_fail "after env kill -s 0 the process was ended by '$n'"
env kill -s 0 "$d" >out 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "env kill -s 0 exited $rc for a process that is gone"
[ ! -s out ] || pc_fail "env kill -s 0 wrote to standard output"
pc_pass
