# A gap of zero given to -e or -i means the default of eight columns.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.014
# MUTANT    pr.e_zero_gap
# MUTANT    pr.i_zero_gap
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.014
LC_ALL=POSIX
export LC_ALL
NL='
'
TAB=$(printf '\t')
S7='       '
printf 'a\tb\n' >t1 || pc_unresolved "cannot create input"
printf 'a%sb\n' "$S7" >s1 || pc_unresolved "cannot create input"
pc_test
got=$(pr -t -e0 t1 2>/dev/null </dev/null; printf x)
[ "$got" = "a${S7}b${NL}x" ] || pc_fail "-e0 did not default the gap to 8"
got=$(pr -t -i0 s1 2>/dev/null </dev/null; printf x)
[ "$got" = "a${TAB}b${NL}x" ] || pc_fail "-i0 did not default the gap to 8"
pc_pass
