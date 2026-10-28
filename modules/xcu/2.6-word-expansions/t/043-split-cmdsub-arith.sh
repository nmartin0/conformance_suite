# The results of unquoted command substitutions and arithmetic expansions
# are split at IFS characters like those of parameter expansions.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.043
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.043
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'IFS=x\n'
	printf 'set -- $(echo axb); echo "$#" "$1" "$2"\n'
	printf 'IFS=2\n'
	printf 'set -- $((123)); echo "$#" "$1" "$2"\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '2 a b\n2 1 3\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
