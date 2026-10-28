# The replacement for a tilde-prefix is treated as quoted, so it is not
# split into fields and not matched against file names, and an empty HOME
# gives one empty field.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.009
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.009
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'HOME=\047pcg*\047\n'
	printf 'echo ~\n'
	printf 'HOME=\047a   b\047\n'
	printf 'set -- ~\n'
	printf 'echo $# "$1"\n'
	printf 'HOME=\n'
	printf 'set -- ~\n'
	printf 'echo $# "[$1]"\n'
} >s.sh || pc_fail "cannot write the test script"
: >pcga || pc_fail "setup failed"
: >pcgb || pc_fail "setup failed"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'pcg*\n1 a   b\n1 []\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
