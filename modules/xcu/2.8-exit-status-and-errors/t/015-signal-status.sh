# When a command is ended by a signal the shell gives it an exit status
# greater than 128.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.8-exit-status-and-errors.015
. "$PC_CORE/pc.sh"
pc_begin XCU.2.8-exit-status-and-errors.015
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf '"$PCSH" -c \047kill -s KILL $$\047\n'
	printf 'r=$?\n'
	printf 'if [ "$r" -gt 128 ]; then echo big; else echo small; fi\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'big\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
