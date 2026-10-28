# Characters inside single quotes keep their literal value, including every
# character that is special elsewhere and a newline; a single quote can be
# added to a word only outside single quotes.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.2-quoting.003
. "$PC_CORE/pc.sh"
pc_begin XCU.2.2-quoting.003
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'printf \047%%s\\n\047 \047$HOME `x` \\n "q" * ? | & ; < > ( ) # ~ !\047 \047a\047\\\047\047b\047\n'
	printf 'printf \047%%s\\n\047 \047p\n'
	printf 'q\047\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '$HOME `x` \\n "q" * ? | & ; < > ( ) # ~ !\na\047b\np\nq\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
