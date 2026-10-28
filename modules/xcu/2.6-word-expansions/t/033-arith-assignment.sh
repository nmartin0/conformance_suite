# The assignment operators = += -= *= /= %= <<= >>= &= ^= |= change the
# variable, the new value is the value of the expression, and the change is
# in effect after the expansion.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.033
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.033
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'x=5\n'
	printf 'echo $((x=3)) $x\n'
	printf 'echo $((x+=2)) $x\n'
	printf 'echo $((x-=1)) $x\n'
	printf 'echo $((x*=3)) $x\n'
	printf 'echo $((x/=2)) $x\n'
	printf 'echo $((x%%=5)) $x\n'
	printf 'echo $((x<<=3)) $x\n'
	printf 'echo $((x>>=1)) $x\n'
	printf 'echo $((x&=6)) $x\n'
	printf 'echo $((x^=3)) $x\n'
	printf 'echo $((x|=8)) $x\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '3 3\n5 5\n4 4\n12 12\n6 6\n1 1\n8 8\n4 4\n4 4\n7 7\n15 15\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
