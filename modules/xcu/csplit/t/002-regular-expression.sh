# csplit /rexp/ starts a new piece at the first line matching the expression, moved by a +n or -n offset.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.csplit.002
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
pc_pass
