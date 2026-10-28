# A {num} operand repeats the preceding regular-expression or line-number split num more times.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.csplit.004
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
pc_pass
