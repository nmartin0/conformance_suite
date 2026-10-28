# sort -o writes the result to the named file instead of standard output, and the file may be an input file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.012
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.012
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'c\nb\na\n' >in || pc_fail "cannot create input"
printf 'z\ny\nx\n' >self || pc_fail "cannot create input"
pc_test
sort -o res in >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-o gave status $rc"
[ -s out ] && pc_fail "-o still wrote to standard output"
got=$(cat res; printf x)
[ "$got" = "a${NL}b${NL}c${NL}x" ] || pc_fail "the -o file does not hold the sorted lines"
sort -o self self >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-o with the input file as output gave status $rc"
got=$(cat self; printf x)
[ "$got" = "x${NL}y${NL}z${NL}x" ] || pc_fail "sorting a file onto itself did not give the sorted lines"
pc_pass
