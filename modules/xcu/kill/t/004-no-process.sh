# kill of a process that does not exist fails.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.004
# MUTANT    kill.gone_ok
# MUTANT    kill.gone_stdout
# MUTANT    kill.zero_gone_ok
# MUTANT    kill.multi_status_first
# MUTANT    kill.multi_status_last
# VARIANT   kill.err_status77
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.004
l=
trap 'command kill -s KILL "$l" 2>/dev/null' EXIT
"${PC_SH:-sh}" -c 'exit 0' &
d=$!
wait "$d"
sleep 30 &
l=$!
pc_test
# a kill that fails for every process would also pass: it must work on a live one
kill -s 0 "$l" >/dev/null 2>&1 || pc_unresolved "kill -s 0 fails for a live process, so its failure for a missing process proves nothing"
# the gone process gets only signals that harm nothing should its number
# have been given to another process meanwhile
kill -s 0 "$d" >out 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "kill -s 0 exited $rc for a process that is gone"
[ ! -s out ] || pc_fail "kill wrote to standard output"
kill -s CONT "$d" >out 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "kill -s CONT exited $rc for a process that is gone"
[ ! -s out ] || pc_fail "kill -s CONT wrote to standard output"
# one operand names no process: the status is not zero, whichever comes first
kill -s 0 "$l" "$d" >out 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "kill -s 0 exited $rc with a live and then a missing process"
[ ! -s out ] || pc_fail "kill wrote to standard output (live, missing)"
kill -s 0 "$d" "$l" >out 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "kill -s 0 exited $rc with a missing and then a live process"
[ ! -s out ] || pc_fail "kill wrote to standard output (missing, live)"
# the same through a direct exec of the utility
env kill -s 0 "$l" >/dev/null 2>&1 || pc_unresolved "env kill -s 0 fails for a live process, so its failure for a missing process proves nothing"
env kill -s 0 "$d" >out 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "env kill -s 0 exited $rc for a process that is gone"
[ ! -s out ] || pc_fail "env kill wrote to standard output"
env kill -s 0 "$l" "$d" >out 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "env kill -s 0 exited $rc with a live and then a missing process"
env kill -s 0 "$d" "$l" >out 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "env kill -s 0 exited $rc with a missing and then a live process"
pc_pass
