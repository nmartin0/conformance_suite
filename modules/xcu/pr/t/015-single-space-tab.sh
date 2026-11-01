# -i also replaces a single space that reaches a tab column.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.015
# MUTANT    pr.i_min2
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.015
LC_ALL=POSIX
export LC_ALL
NL='
'
TAB=$(printf '\t')
printf '1234567 b\n' >s4 || pc_unresolved "cannot create input"
pc_test
got=$(pr -t -i s4 2>/dev/null </dev/null; printf x)
[ "$got" = "1234567${TAB}b${NL}x" ] || pc_fail "-i did not replace a single space reaching column 9"
pc_pass
