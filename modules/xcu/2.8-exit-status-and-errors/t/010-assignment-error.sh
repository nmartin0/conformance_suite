# An attempt to assign to a read-only variable is a variable assignment
# error: a non-interactive shell writes a diagnostic and exits with a status
# from 1 to 125.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.8-exit-status-and-errors.010
. "$PC_CORE/pc.sh"
pc_begin XCU.2.8-exit-status-and-errors.010
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'readonly r=1\n'
	printf 'r=2\n'
	printf 'echo notreached\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf ''; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
{ [ "$rc" -ge 1 ] && [ "$rc" -le 125 ]; } || pc_fail "exit status $rc, expected 1 to 125"
[ -s err ] || pc_fail "no diagnostic was written to standard error"
pc_pass
