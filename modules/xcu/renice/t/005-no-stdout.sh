# renice writes nothing to standard output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.renice.005
. "$PC_CORE/pc.sh"
pc_begin XCU.renice.005
LC_ALL=C; export LC_ALL
bg=
trap '[ -n "$bg" ] && { kill "$bg" 2>/dev/null; wait "$bg" 2>/dev/null; }' EXIT
sleep 301 </dev/null >/dev/null 2>&1 &
bg=$!
pc_test
renice -n 1 -p "$bg" >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "renice exited with status $rc"
[ -s out ] && pc_fail "renice wrote to standard output"
pc_pass
