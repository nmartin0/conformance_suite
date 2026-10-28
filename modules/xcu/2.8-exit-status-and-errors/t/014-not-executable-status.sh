# A command that is found but is not an executable utility (a file without
# execute permission, a directory) has exit status 126.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.8-exit-status-and-errors.014
. "$PC_CORE/pc.sh"
pc_begin XCU.2.8-exit-status-and-errors.014
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf ': >pcnoexec\n'
	printf 'mkdir pcdir\n'
	printf '(./pcnoexec) 2>/dev/null; echo $?\n'
	printf '(./pcdir) 2>/dev/null; echo $?\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '126\n126\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
