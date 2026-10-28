# Otherwise the exit status of a command is the value it passed to exit, 0
# to 255 (for a program the remainder modulo 256), as seen by wait.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.8-exit-status-and-errors.016
. "$PC_CORE/pc.sh"
pc_begin XCU.2.8-exit-status-and-errors.016
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf '"$PCSH" -c \047exit 0\047; echo $?\n'
	printf '"$PCSH" -c \047exit 7\047; echo $?\n'
	printf '"$PCSH" -c \047exit 255\047; echo $?\n'
	printf '(exit 42); echo $?\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '0\n7\n255\n42\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
