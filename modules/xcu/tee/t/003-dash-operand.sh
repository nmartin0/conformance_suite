# A file operand of - names a file called -, not standard output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tee.003
. "$PC_CORE/pc.sh"
pc_begin XCU.tee.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'data\n' >in || pc_fail "cannot create input"
pc_test
tee - <in >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "tee - gave status $rc"
[ -f ./- ] || pc_fail "no file named - was created"
got=$(cat ./-; printf x)
[ "$got" = "data${NL}x" ] || pc_fail "the file named - does not hold the input"
got=$(cat out; printf x)
[ "$got" = "data${NL}x" ] || pc_fail "standard output did not receive exactly one copy"
pc_pass
