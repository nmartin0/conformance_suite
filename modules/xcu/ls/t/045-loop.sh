# ls -RL: a directory loop is detected
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.045
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.045
LC_ALL=POSIX; export LC_ALL
pc_pid=
trap '[ -n "$pc_pid" ] && kill -9 "$pc_pid" 2>/dev/null' EXIT
mkdir -p d/e || pc_unresolved "cannot create d/e"
: >d/e/f && ln -s .. d/e/up || pc_unresolved "cannot create the loop"
pc_test
# A limit on the file size keeps an ls that does not stop from filling the disk.
( ulimit -f 4096 2>/dev/null; exec ls -RL d ) >out 2>err </dev/null &
pc_pid=$!
i=0
while kill -0 "$pc_pid" 2>/dev/null && [ "$i" -lt 30 ]; do
	sleep 1
	i=$((i + 1))
done
if kill -0 "$pc_pid" 2>/dev/null; then
	kill -9 "$pc_pid" 2>/dev/null
	wait "$pc_pid" 2>/dev/null
	pc_pid=
	pc_fail "ls -RL still ran 30 seconds after entering the directory loop"
fi
wait "$pc_pid" 2>/dev/null
pc_pid=
[ -s err ] || pc_fail "ls stopped (or was stopped) without writing a diagnostic to standard error"
sz=$(wc -c <out); sz=$((sz + 0))
[ "$sz" -lt 2000000 ] || pc_fail "the output grew to $sz octets: the loop was not detected"
pc_pass
