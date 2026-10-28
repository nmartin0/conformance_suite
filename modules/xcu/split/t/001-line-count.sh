# split -l n writes pieces of n lines to files xaa, xab, ..., the last holding the remainder.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.split.001
. "$PC_CORE/pc.sh"
pc_begin XCU.split.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 6 7 >in || pc_fail "cannot create input"
pc_test
split -l 3 in >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "split -l 3 gave status $rc"
[ -s out ] && pc_fail "split wrote to standard output"
got=$(cat xaa; printf x)
[ "$got" = "1${NL}2${NL}3${NL}x" ] || pc_fail "xaa does not hold lines 1 to 3"
got=$(cat xab; printf x)
[ "$got" = "4${NL}5${NL}6${NL}x" ] || pc_fail "xab does not hold lines 4 to 6"
got=$(cat xac; printf x)
[ "$got" = "7${NL}x" ] || pc_fail "xac does not hold line 7"
[ -e xad ] && pc_fail "a fourth file was created"
pc_pass
