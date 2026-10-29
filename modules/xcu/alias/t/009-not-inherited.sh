# A separate shell does not inherit aliases.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.009
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.009
pc_test
alias pcal9='echo x'
out=$(${PC_SH:-sh} -c 'echo ran; alias pcal9' 2>/dev/null)
rc=$?
# the first line shows that the shell ran at all
[ "$out" = ran ] || pc_fail "a new shell wrote '$out', wanted only 'ran' (exit $rc)"
[ "$rc" -gt 0 ] || pc_fail "a new shell knows the alias (exit $rc)"
pc_pass
