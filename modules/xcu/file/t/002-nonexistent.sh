# For a file that does not exist, file says it cannot open it and still exits 0.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.file.002
. "$PC_CORE/pc.sh"
pc_begin XCU.file.002
pc_test
file nonexistent >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "file exited with status $rc"
l=$(cat out)
case $l in
"nonexistent: "*"cannot open"*) ;;
*) pc_fail "line is: $l" ;;
esac
pc_pass
