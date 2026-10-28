# A shell variable initialized from the environment is marked for export at
# once, so a later plain assignment to it is passed on to programs the shell
# runs.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.5-parameters.012
. "$PC_CORE/pc.sh"
pc_begin XCU.2.5-parameters.012
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'echo "$PCENV"\n'
	printf 'PCENV=changed\n'
	printf '"$PCSH" -c \047echo "$PCENV"\047\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
PCENV=hello "$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'hello\nchanged\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
