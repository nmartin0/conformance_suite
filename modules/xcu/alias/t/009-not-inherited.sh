# A separate shell does not inherit aliases.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.009
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.009
pc_test
alias pcal9='echo x'
out=$(${PC_SH:-sh} -c 'alias pcal9' 2>/dev/null)
rc=$?
[ "$rc" -gt 0 ] || pc_fail "a new shell knows the alias (exit $rc)"
[ -z "$out" ] || pc_fail "a new shell wrote '$out'"
pc_pass
