# csplit %rexp% moves to the matching line without writing a file for the lines passed over.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.csplit.003
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
pc_pass
