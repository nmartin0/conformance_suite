# Braces are optional around a name except where the next character could
# continue it, and an unbraced name is the longest valid name; for
# positional parameters of more than one digit braces are needed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.010
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.010
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'a=1\n'
	printf 'set -- 2\n'
	printf 'echo ${a}b-$ab-${1}0-${10}-$10\n'
	printf 'x=1; xy=2\n'
	printf 'echo $xy ${x}y\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '1b--20--20\n2 1y\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
