# csplit /rexp/ starts a new piece at the first line matching the expression, moved by a +n or -n offset.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.csplit.002
# MUTANT    csplit.off_plus_ignored
# MUTANT    csplit.off_minus_ignored
# MUTANT    csplit.off_plus_extra
# MUTANT    csplit.off_plus_less
# MUTANT    csplit.off_minus_extra
# MUTANT    csplit.off_minus_less
# MUTANT    csplit.off_sign_swapped
# MUTANT    csplit.off_first_digit
# MUTANT    csplit.off_ignored
# MUTANT    csplit.plus_rejected
# MUTANT    csplit.match_last
# MUTANT    csplit.match_second
# MUTANT    csplit.match_from_cur
# MUTANT    csplit.match_ere
# MUTANT    csplit.match_literal
# MUTANT    csplit.match_icase
# MUTANT    csplit.slash_ends_expr
# MUTANT    csplit.match_inclusive
# MUTANT    csplit.match_exclusive_extra
. "$PC_CORE/pc.sh"
pc_begin XCU.csplit.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a1\na2\nb1\nb2\nc1\nc2\n' >f || pc_fail "cannot create input"
pc_test
csplit -s f '/^b1/' >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit /^b1/ gave status $rc"
got=$(cat xx00; printf x)
[ "$got" = "a1${NL}a2${NL}x" ] || pc_fail "/^b1/: xx00 is wrong"
got=$(cat xx01; printf x)
[ "$got" = "b1${NL}b2${NL}c1${NL}c2${NL}x" ] || pc_fail "/^b1/: xx01 is wrong"
rm -f xx00 xx01 xx02
csplit -s f '/^b1/+1' >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit /^b1/+1 gave status $rc"
got=$(cat xx00; printf x)
[ "$got" = "a1${NL}a2${NL}b1${NL}x" ] || pc_fail "/^b1/+1: xx00 is wrong"
got=$(cat xx01; printf x)
[ "$got" = "b2${NL}c1${NL}c2${NL}x" ] || pc_fail "/^b1/+1: xx01 is wrong"
rm -f xx00 xx01 xx02
csplit -s f '/^b2/-1' >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit /^b2/-1 gave status $rc"
got=$(cat xx00; printf x)
[ "$got" = "a1${NL}a2${NL}x" ] || pc_fail "/^b2/-1: xx00 is wrong"
got=$(cat xx01; printf x)
[ "$got" = "b1${NL}b2${NL}c1${NL}c2${NL}x" ] || pc_fail "/^b2/-1: xx01 is wrong"
rm -f xx00 xx01 xx02
# the search for the next operand starts after the current line
csplit -s f '/^b1/' '/^b/' >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit /^b1/ /^b/ gave status $rc"
got=$(cat xx00; printf x)
[ "$got" = "a1${NL}a2${NL}x" ] || pc_fail "/^b1/ /^b/: xx00 is wrong"
got=$(cat xx01; printf x)
[ "$got" = "b1${NL}x" ] || pc_fail "/^b1/ /^b/: xx01 is wrong"
got=$(cat xx02; printf x)
[ "$got" = "b2${NL}c1${NL}c2${NL}x" ] || pc_fail "/^b1/ /^b/: xx02 is wrong"
rm -f xx00 xx01 xx02
# the first matching line is used, not a later one
csplit -s f '/[bc]/' >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit /[bc]/ gave status $rc"
got=$(cat xx00; printf x)
[ "$got" = "a1${NL}a2${NL}x" ] || pc_fail "/[bc]/: xx00 is wrong"
rm -f xx00 xx01 xx02
# offsets of two digits
i=0
while [ "$i" -lt 20 ]; do
	i=$((i + 1))
	printf 'l%02d\n' "$i"
done >g20
csplit -s g20 '/^l05/+12' >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit /^l05/+12 gave status $rc"
[ "$(wc -l <xx00)" -eq 16 ] || pc_fail "/^l05/+12: xx00 does not end at line 16"
[ "$(head -n 1 xx01)" = l17 ] || pc_fail "/^l05/+12: xx01 does not start at line 17"
rm -f xx00 xx01 xx02
csplit -s g20 '/^l15/-12' >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit /^l15/-12 gave status $rc"
[ "$(wc -l <xx00)" -eq 2 ] || pc_fail "/^l15/-12: xx00 does not end at line 2"
[ "$(head -n 1 xx01)" = l03 ] || pc_fail "/^l15/-12: xx01 does not start at line 3"
rm -f xx00 xx01 xx02
# a basic regular expression: + is an ordinary character, a group and the
# case of letters count
printf 'top\nB1\nbb\nb+\nb1\nend\n' >h
csplit -s h '/b+/' >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit /b+/ gave status $rc"
got=$(cat xx01; printf x)
[ "$got" = "b+${NL}b1${NL}end${NL}x" ] || pc_fail "/b+/ did not start the piece at the line b+"
rm -f xx00 xx01 xx02
csplit -s h '/^b1/' >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit /^b1/ on mixed case gave status $rc"
got=$(cat xx01; printf x)
[ "$got" = "b1${NL}end${NL}x" ] || pc_fail "/^b1/ matched a line of another case"
rm -f xx00 xx01 xx02
csplit -s h '/^\(b\)1/' >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit with a group gave status $rc"
got=$(cat xx01; printf x)
[ "$got" = "b1${NL}end${NL}x" ] || pc_fail "the group expression did not start the piece at b1"
rm -f xx00 xx01 xx02
# a slash inside the expression is written \/
printf 'a1\na2\na/x\na3\n' >sl
csplit -s sl '/^a\/x/' >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit with \\/ in the expression gave status $rc"
got=$(cat xx00; printf x)
[ "$got" = "a1${NL}a2${NL}x" ] || pc_fail "/^a\\/x/: xx00 is wrong"
got=$(cat xx01; printf x)
[ "$got" = "a/x${NL}a3${NL}x" ] || pc_fail "/^a\\/x/: xx01 is wrong"
pc_pass
