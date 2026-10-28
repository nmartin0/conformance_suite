# csplit -f sets the file name prefix and -n the number of digits.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.csplit.005
. "$PC_CORE/pc.sh"
pc_begin XCU.csplit.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 >in10 || pc_fail "cannot create input"
pc_test
csplit -s -f part -n 3 in10 5 >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit -f part -n 3 gave status $rc"
got=$(cat part000; printf x)
[ "$got" = "1${NL}2${NL}3${NL}4${NL}x" ] || pc_fail "part000 is wrong"
got=$(cat part001; printf x)
[ "$got" = "5${NL}6${NL}7${NL}8${NL}9${NL}10${NL}x" ] || pc_fail "part001 is wrong"
[ -e xx00 ] && pc_fail "a file with the default prefix was created"
[ -e part00 ] && pc_fail "a file with the default two digits was created"
csplit -s -n 4 in10 5 >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit -n 4 gave status $rc"
[ -s xx0000 ] && [ -s xx0001 ] || pc_fail "-n 4 did not give xx0000 and xx0001"
pc_pass
