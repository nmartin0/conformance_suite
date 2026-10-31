# sort -n orders by the arithmetic value of an initial numeric string; equal values fall back to comparing the whole line.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.003
# MUTANT    sort.n_ignored
# MUTANT    sort.n_no_minus
# MUTANT    sort.n_no_frac
# MUTANT    sort.n_no_blanks
# MUTANT    sort.n_text_last
# MUTANT    sort.n_minus0_less
# MUTANT    sort.n_neg_frac_wrong
# MUTANT    sort.n_tie_stable
# MUTANT    sort.n_tie_forward
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '10\n9\n-2\n  3\n0003\n2.5\nx\n' >in || pc_fail "cannot create input"
printf '03.9\n3.10\n-1.5\n-1\n-2\n' >in2 || pc_fail "cannot create input"
printf ' 0\n-0\n0.5\n-0.0\n' >in3 || pc_fail "cannot create input"
pc_test
got=$(sort -n in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "-2${NL}x${NL}2.5${NL}  3${NL}0003${NL}9${NL}10${NL}rc=0" ] || pc_fail "-n order is wrong"
got=$(sort -nr in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "10${NL}9${NL}0003${NL}  3${NL}2.5${NL}x${NL}-2${NL}rc=0" ] || pc_fail "-nr order is wrong"
got=$(sort -n in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "-2${NL}-1.5${NL}-1${NL}3.10${NL}03.9${NL}rc=0" ] || pc_fail "-n order of fractions and negative numbers is wrong"
got=$(sort -n in3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = " 0${NL}-0${NL}-0.0${NL}0.5${NL}rc=0" ] || pc_fail "-n: a sign on a zero changed the order"
pc_pass
