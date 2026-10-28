# realpath -e with an empty operand fails: nothing on standard output, a diagnostic, non-zero status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.realpath.007
. "$PC_CORE/pc.sh"
pc_begin XCU.realpath.007
pc_test
realpath -e '' >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "realpath -e '' exited 0"
[ ! -s out ] || pc_fail "output written"
[ -s err ] || pc_fail "no diagnostic"
pc_pass
