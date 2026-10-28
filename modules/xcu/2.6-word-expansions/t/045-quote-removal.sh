# Quote removal deletes the quoting backslashes, single quotes and double
# quotes of the word as written, but not a quote character that was itself
# quoted or that came out of an expansion.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.045
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.045
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'printf \047[%%s]\047 "a"\047b\047c\\d "\\\\" \047\\\047 "\047" \047"\047 \\\047 \\"; echo\n'
	printf 'x="\047a\047"\n'
	printf 'printf \047[%%s]\047 $x "$x"; echo\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '[abcd][\\][\\][\047]["][\047]["]\n[\047a\047][\047a\047]\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
