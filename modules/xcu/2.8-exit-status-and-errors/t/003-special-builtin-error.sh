# An error in a special built-in utility executed directly makes a
# non-interactive shell exit with a nonzero status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.8-exit-status-and-errors.003
. "$PC_CORE/pc.sh"
pc_begin XCU.2.8-exit-status-and-errors.003
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'readonly r=1\n'
	printf 'unset r\n'
	printf 'echo notreached\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf ''; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
[ "$rc" -ne 0 ] || pc_fail "exit status was 0, expected nonzero"
pc_pass
