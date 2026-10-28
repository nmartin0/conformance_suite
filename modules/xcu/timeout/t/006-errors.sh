# Exit statuses 125, 126 and 127.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.timeout.006
. "$PC_CORE/pc.sh"
pc_begin XCU.timeout.006
printf 'exit 0\n' >noexec || pc_unresolved "setup: cannot create a file"
chmod 644 noexec || pc_unresolved "setup: chmod failed"
pc_test
timeout 1 pc_no_such_utility_zz </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 127 ] || pc_fail "timeout of a missing utility exited $rc, wanted 127"
timeout 1 ./noexec </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 126 ] || pc_fail "timeout of a file that cannot be run exited $rc, wanted 126"
timeout abc true </dev/null >/dev/null 2>&1
rc=$?
[ "$rc" -eq 125 ] || pc_fail "timeout abc true exited $rc, wanted 125"
pc_pass
