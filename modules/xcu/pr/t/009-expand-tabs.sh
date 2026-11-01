# -e expands tabs, -i replaces spaces that reach tab columns by tabs.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.009
# MUTANT    pr.e_ignored
# MUTANT    pr.e_gap_ignored
# MUTANT    pr.e_default4
# MUTANT    pr.e_gap_plus1
# MUTANT    pr.e_char_ignored
# MUTANT    pr.e_fixed8
# MUTANT    pr.e_boundary_noexp
# MUTANT    pr.i_ignored
# MUTANT    pr.i_char_ignored
# MUTANT    pr.i_gap_ignored
# MUTANT    pr.i_all_spaces
# MUTANT    pr.i_once
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.009
LC_ALL=POSIX
export LC_ALL
NL='
'
TAB=$(printf '\t')
S7='       '
S8='        '
printf 'a\tb\n' >t1 || pc_unresolved "cannot create input"
printf 'aXb\n' >t2 || pc_unresolved "cannot create input"
printf 'ab\tc\n' >t3 || pc_unresolved "cannot create input"
printf '1234567\tb\n' >t4 || pc_unresolved "cannot create input"
printf '12345678\tb\n' >t5 || pc_unresolved "cannot create input"
printf 'a\tb\tc\n' >t6 || pc_unresolved "cannot create input"
printf 'aXb\n' >t7 || pc_unresolved "cannot create input"
printf 'a%sb\n' "$S7" >s1 || pc_unresolved "cannot create input"
printf 'a   b\n' >s2 || pc_unresolved "cannot create input"
printf 'a b\n' >s3 || pc_unresolved "cannot create input"
printf 'a%sb%sc\n' "$S7" "$S7" >s5 || pc_unresolved "cannot create input"
pc_test
got=$(pr -t -e t1 2>/dev/null </dev/null; printf x)
[ "$got" = "a${S7}b${NL}x" ] || pc_fail "-e did not expand a tab to column 9"
got=$(pr -t -e3 t1 2>/dev/null </dev/null; printf x)
[ "$got" = "a  b${NL}x" ] || pc_fail "-e3 did not expand a tab to column 4"
got=$(pr -t -eX t2 2>/dev/null </dev/null; printf x)
[ "$got" = "a${S7}b${NL}x" ] || pc_fail "-eX did not treat X as the input tab"
got=$(pr -t -eX3 t7 2>/dev/null </dev/null; printf x)
[ "$got" = "a  b${NL}x" ] || pc_fail "-eX3 did not treat X as the input tab with a gap of 3"
got=$(pr -t -e t3 2>/dev/null </dev/null; printf x)
[ "$got" = "ab      c${NL}x" ] || pc_fail "-e did not expand a tab after two characters to column 9"
got=$(pr -t -e t4 2>/dev/null </dev/null; printf x)
[ "$got" = "1234567 b${NL}x" ] || pc_fail "-e did not expand a tab in column 8 to column 9"
got=$(pr -t -e t5 2>/dev/null </dev/null; printf x)
[ "$got" = "12345678${S8}b${NL}x" ] || pc_fail "-e did not expand a tab in column 9 to column 17"
got=$(pr -t -e t6 2>/dev/null </dev/null; printf x)
[ "$got" = "a${S7}b${S7}c${NL}x" ] || pc_fail "-e did not expand the second tab to column 17"
got=$(pr -t -i s1 2>/dev/null </dev/null; printf x)
[ "$got" = "a${TAB}b${NL}x" ] || pc_fail "-i did not replace spaces reaching column 9 by a tab"
got=$(pr -t -i:4 s2 2>/dev/null </dev/null; printf x)
[ "$got" = "a:b${NL}x" ] || pc_fail "-i:4 did not replace spaces reaching column 5 by a colon"
got=$(pr -t -i4 s2 2>/dev/null </dev/null; printf x)
[ "$got" = "a${TAB}b${NL}x" ] || pc_fail "-i4 did not take 4 as the gap"
got=$(pr -t -i s3 2>/dev/null </dev/null; printf x)
[ "$got" = "a b${NL}x" ] || pc_fail "-i changed a space that reaches no tab column"
got=$(pr -t -i s2 2>/dev/null </dev/null; printf x)
[ "$got" = "a   b${NL}x" ] || pc_fail "-i changed spaces that reach no tab column"
got=$(pr -t -i s5 2>/dev/null </dev/null; printf x)
[ "$got" = "a${TAB}b${TAB}c${NL}x" ] || pc_fail "-i did not replace spaces reaching columns 9 and 17"
pc_pass
