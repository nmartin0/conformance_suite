# With -P, a component starting with a hyphen and an empty pathname are diagnosed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pathchk.007
. "$PC_CORE/pc.sh"
pc_begin XCU.pathchk.007
pc_test
pathchk -P a/b-c >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "pathchk -P exited with status $rc for a good name"
pathchk -P a/-b >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "pathchk -P exited 0 for a component starting with a hyphen"
pathchk -P -- -x >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "pathchk -P exited 0 for a leading hyphen"
pathchk -P '' >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "pathchk -P exited 0 for an empty pathname"
[ -s err ] || pc_fail "no diagnostic"
pc_pass
