# An empty regular file is described with the word empty.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.file.005
. "$PC_CORE/pc.sh"
pc_begin XCU.file.005
: >e || pc_fail "cannot create e"
pc_test
file e >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "file exited with status $rc"
l=$(cat out)
case $l in
"e: "*empty*) ;;
*) pc_fail "line is: $l" ;;
esac
pc_pass
