# pathchk accepts pathnames whose components need not exist as long as such a file could be created.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pathchk.001
. "$PC_CORE/pc.sh"
pc_begin XCU.pathchk.001
mkdir real || pc_fail "cannot create real"
pc_test
pathchk real/new/deeper/file >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "pathchk of a creatable pathname exited with status $rc"
pathchk . real "$(pwd -P)/real/x" >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "pathchk of existing pathnames exited with status $rc"
[ ! -s out ] || pc_fail "pathchk wrote to standard output"
pc_pass
