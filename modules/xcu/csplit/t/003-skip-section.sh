# csplit %rexp% moves to the matching line without writing a file for the lines passed over.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.csplit.003
# MUTANT    csplit.skip_writes_file
# MUTANT    csplit.skip_gap
# MUTANT    csplit.skip_prints_size
# MUTANT    csplit.skip_not_advances
# MUTANT    csplit.skip_cur_plus1
# MUTANT    csplit.skip_cur_minus1
# MUTANT    csplit.skip_ignores_offset
# MUTANT    csplit.percent_ends_expr
. "$PC_CORE/pc.sh"
pc_begin XCU.csplit.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a1\na2\nb1\nb2\nc1\nc2\n' >f || pc_fail "cannot create input"
pc_test
got=$(csplit f '%^b1%' '/^c1/' 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "6${NL}6${NL}rc=0" ] || pc_fail "the size lines are not 6 and 6"
got=$(cat xx00; printf x)
[ "$got" = "b1${NL}b2${NL}x" ] || pc_fail "xx00 does not start at the line matched by %^b1%"
got=$(cat xx01; printf x)
[ "$got" = "c1${NL}c2${NL}x" ] || pc_fail "xx01 is wrong"
[ -e xx02 ] && pc_fail "a file was written for the skipped lines"
rm -f xx00 xx01 xx02
got=$(csplit f '%^b1%+1' '/^c2/' 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "6${NL}3${NL}rc=0" ] || pc_fail "the size lines with an offset are not 6 and 3"
got=$(cat xx00; printf x)
[ "$got" = "b2${NL}c1${NL}x" ] || pc_fail "xx00 does not start at the line after %^b1%+1"
got=$(cat xx01; printf x)
[ "$got" = "c2${NL}x" ] || pc_fail "xx01 is wrong after an offset skip"
[ -e xx02 ] && pc_fail "a file was written for the skipped lines (offset)"
rm -f xx00 xx01 xx02
# a percent sign inside the expression is written \%
printf 'x1\npp\np%%q\nx2\nx3\n' >pc
csplit -s pc '%^p\%q%' '/^x3/' >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit with \\% in the expression gave status $rc"
got=$(cat xx00; printf x)
[ "$got" = "p%q${NL}x2${NL}x" ] || pc_fail "xx00 is wrong after skipping to p%q"
got=$(cat xx01; printf x)
[ "$got" = "x3${NL}x" ] || pc_fail "xx01 is wrong after skipping to p%q"
pc_pass
