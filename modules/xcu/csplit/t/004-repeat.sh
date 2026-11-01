# A {num} operand repeats the preceding regular-expression or line-number split num more times.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.csplit.004
# MUTANT    csplit.rep_extra
# MUTANT    csplit.rep_less
# MUTANT    csplit.rep_once
# MUTANT    csplit.rep_ignored
# MUTANT    csplit.rep_first_digit
# MUTANT    csplit.rep_skip_ignored
# MUTANT    csplit.rep_offset_dropped
# MUTANT    csplit.rep_ln_same
# MUTANT    csplit.match_from_cur
. "$PC_CORE/pc.sh"
pc_begin XCU.csplit.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'h\ns1\na\ns2\nb\ns3\nc\n' >f || pc_fail "cannot create input"
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 >in10 || pc_fail "cannot create input"
pc_test
csplit -s f '/^s/' '{2}' >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit /^s/ {2} gave status $rc"
got=$(cat xx00; printf x)
[ "$got" = "h${NL}x" ] || pc_fail "xx00 is wrong after /^s/ {2}"
got=$(cat xx01; printf x)
[ "$got" = "s1${NL}a${NL}x" ] || pc_fail "xx01 is wrong after /^s/ {2}"
got=$(cat xx02; printf x)
[ "$got" = "s2${NL}b${NL}x" ] || pc_fail "xx02 is wrong after /^s/ {2}"
got=$(cat xx03; printf x)
[ "$got" = "s3${NL}c${NL}x" ] || pc_fail "xx03 is wrong after /^s/ {2}"
[ -e xx04 ] && pc_fail "a fifth file was created"
rm -f xx00 xx01 xx02 xx03
csplit -s in10 3 '{2}' >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit 3 {2} gave status $rc"
got=$(cat xx00; printf x)
[ "$got" = "1${NL}2${NL}x" ] || pc_fail "xx00 is wrong after 3 {2}"
got=$(cat xx01; printf x)
[ "$got" = "3${NL}4${NL}5${NL}x" ] || pc_fail "xx01 is wrong after 3 {2}"
got=$(cat xx02; printf x)
[ "$got" = "6${NL}7${NL}8${NL}x" ] || pc_fail "xx02 is wrong after 3 {2}"
got=$(cat xx03; printf x)
[ "$got" = "9${NL}10${NL}x" ] || pc_fail "xx03 is wrong after 3 {2}"
[ -e xx04 ] && pc_fail "a fifth file was created after 3 {2}"
rm -f xx00 xx01 xx02 xx03
# a repeat count of two digits
i=0
printf 'h\n' >many
while [ "$i" -lt 12 ]; do
	i=$((i + 1))
	printf 's%s\nx%s\n' "$i" "$i"
done >>many
csplit -s many '/^s/' '{11}' >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit /^s/ {11} gave status $rc"
got=$(cat xx12; printf x)
[ "$got" = "s12${NL}x12${NL}x" ] || pc_fail "xx12 is wrong after {11}"
[ -e xx13 ] && pc_fail "a file xx13 was created after {11}"
rm -f xx[0-9][0-9]
# the repeat operand also follows a skip operand
csplit -s f '%^s%' '{1}' >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit %^s% {1} gave status $rc"
got=$(cat xx00; printf x)
[ "$got" = "s2${NL}b${NL}s3${NL}c${NL}x" ] || pc_fail "xx00 is wrong after %^s% {1}"
[ -e xx01 ] && pc_fail "a second file was created after %^s% {1}"
rm -f xx00 xx01
# the repeated operation keeps its offset
csplit -s f '/^s/+1' '{1}' >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit /^s/+1 {1} gave status $rc"
got=$(cat xx00; printf x)
[ "$got" = "h${NL}s1${NL}x" ] || pc_fail "xx00 is wrong after /^s/+1 {1}"
got=$(cat xx01; printf x)
[ "$got" = "a${NL}s2${NL}x" ] || pc_fail "xx01 is wrong after /^s/+1 {1}"
got=$(cat xx02; printf x)
[ "$got" = "b${NL}s3${NL}c${NL}x" ] || pc_fail "xx02 is wrong after /^s/+1 {1}"
pc_pass
