# A word with quoting is not substituted.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.011
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.011
pc_test
alias pcl4='echo aliased'
got=$(eval '\pcl4' 2>/dev/null)
rc=$?
[ "$got" != aliased ] || pc_fail "backslash-quoted word was substituted"
[ "$rc" -eq 127 ] || pc_fail "quoted word: exit $rc, wanted 127 (no such command)"
got=$(eval "'pcl4'" 2>/dev/null)
rc=$?
[ "$got" != aliased ] || pc_fail "single-quoted word was substituted"
[ "$rc" -eq 127 ] || pc_fail "single-quoted word: exit $rc, wanted 127 (no such command)"
pc_pass
