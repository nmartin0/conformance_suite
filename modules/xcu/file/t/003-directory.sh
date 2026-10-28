# A directory is described with the word directory.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.file.003
. "$PC_CORE/pc.sh"
pc_begin XCU.file.003
mkdir d || pc_fail "cannot create d"
pc_test
file d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "file exited with status $rc"
l=$(cat out)
case $l in
"d: "*directory*) ;;
*) pc_fail "line is: $l" ;;
esac
pc_pass
