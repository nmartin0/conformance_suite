# Arithmetic expansion evaluates the integer operators + - * / % and unary +
# and - with C precedence, parentheses, and C rules for division and
# remainder (the quotient is truncated toward zero and the remainder has the
# sign of the dividend).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.030
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.030
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'echo $((1+2*3)) $(((1+2)*3)) $((7/2)) $((-7/2)) $((7%%3)) $((-7%%3)) $((7%%-3)) $((2-5)) $((- 3)) $((+4))\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '7 9 3 -3 1 -1 1 -3 -3 4\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
