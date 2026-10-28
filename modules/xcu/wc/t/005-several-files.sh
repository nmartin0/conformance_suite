# With several files wc writes a line for each and a final line with the word total and the sums.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wc.005
. "$PC_CORE/pc.sh"
pc_begin XCU.wc.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'one two\nthree\n\nfour five six\n' >f || pc_fail "cannot create input"
printf 'a b\n' >g || pc_fail "cannot create input"
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
pc_pass
