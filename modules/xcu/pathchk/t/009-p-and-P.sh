# -p and -P can be combined and both sets of checks apply.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pathchk.009
. "$PC_CORE/pc.sh"
pc_begin XCU.pathchk.009
pc_test
pathchk -p -P a/b >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "pathchk -p -P exited with status $rc for a good name"
pathchk -pP a/-b >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "pathchk -pP exited 0 for a component starting with a hyphen"
pathchk -p -P 'a b' >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "pathchk -p -P exited 0 for a name with a blank"
pc_pass
