# A command that is not found makes the shell write a diagnostic to standard
# error and the status is 127, also when the shell then exits.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.8-exit-status-and-errors.013
. "$PC_CORE/pc.sh"
pc_begin XCU.2.8-exit-status-and-errors.013
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'pcnosuchcmd\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf ''; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
[ "$rc" -eq 127 ] || pc_fail "exit status $rc, expected 127"
[ -s err ] || pc_fail "no diagnostic was written to standard error"
pc_pass
