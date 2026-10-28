# kill -s 0 sends nothing and tests the process.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.003
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.003
p=
trap 'kill -s KILL "$p" 2>/dev/null' EXIT
"${PC_SH:-sh}" -c 'exit 0' &
d=$!
wait "$d"
pc_test
sleep 100 &
p=$!
kill -s 0 "$p" 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "kill -s 0 exited $rc for a live process"
kill -s KILL "$p" 2>/dev/null || pc_fail "kill -s KILL failed"
wait "$p"
n=$(kill -l "$?")
[ "$n" = KILL ] || pc_fail "the process was ended by '$n': signal 0 may have been delivered as a real signal"
kill -s 0 "$d" 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "kill -s 0 exited $rc for a process that is gone"
pc_pass
