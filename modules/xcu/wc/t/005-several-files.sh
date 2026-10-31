# With several files wc writes a line for each and a final line with the word total and the sums.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wc.005
# MUTANT    wc.total_missing
# MUTANT    wc.total_single
# MUTANT    wc.total_name
# MUTANT    wc.total_first
# MUTANT    wc.total_last_only
# MUTANT    wc.total_first_only
# MUTANT    wc.total_two_only
# MUTANT    wc.total_all_cols
# MUTANT    wc.reverse_files
# MUTANT    wc.name_missing
# MUTANT    wc.l_off
# VARIANT   wc.padded
. "$PC_CORE/pc.sh"
pc_begin XCU.wc.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'one two\nthree\n\nfour five six\n' >f || pc_fail "cannot create input"
printf 'a b\n' >g || pc_fail "cannot create input"
printf 'x\n' >h || pc_fail "cannot create input"
pc_test
out=$(wc f g 2>/dev/null </dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "wc f g gave status $rc"
set -- $out
[ $# -eq 12 ] || pc_fail "output is not three lines of four fields"
[ "$1 $2 $3 $4" = "4 6 29 f" ] || pc_fail "line for f is wrong"
[ "$5 $6 $7 $8" = "1 2 4 g" ] || pc_fail "line for g is wrong"
[ "$9 ${10} ${11} ${12}" = "5 8 33 total" ] || pc_fail "total line is wrong"
out=$(wc -l f g 2>/dev/null </dev/null)
set -- $out
[ "$*" = "4 f 1 g 5 total" ] || pc_fail "wc -l f g did not end with the total of the line counts"
out=$(wc -wc f g 2>/dev/null </dev/null)
set -- $out
[ "$*" = "6 29 f 2 4 g 8 33 total" ] || pc_fail "wc -wc f g did not end with the totals of the two columns"
out=$(wc f g h 2>/dev/null </dev/null)
set -- $out
[ $# -eq 16 ] || pc_fail "output for three files is not four lines of four fields"
[ "$9 ${10} ${11} ${12}" = "1 1 2 h" ] || pc_fail "line for h is wrong"
[ "${13} ${14} ${15} ${16}" = "6 9 35 total" ] || pc_fail "total line for three files is wrong"
out=$(wc g 2>/dev/null </dev/null)
set -- $out
[ $# -eq 4 ] || pc_fail "one operand did not give a single entry"
pc_pass
