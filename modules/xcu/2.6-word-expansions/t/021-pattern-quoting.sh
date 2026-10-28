# In the substring forms, pattern characters written unquoted, or arriving
# from an unquoted expansion, are special, also when the whole expansion is
# inside double quotes, and quoting a character inside the braces (or
# quoting the expansion that supplies the pattern) makes it literal.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.021
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.021
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'x=\047a*b\047\n'
	printf 'r1=${x#"a*"}; r2=${x#\047a*\047}; r3="${x#a\\*}"; r4="${x#\047a*\047}"\n'
	printf 'echo "$r1 $r2 $r3 $r4"\n'
	printf 'x=aXb\n'
	printf 'r1="${x#\047a*\047}"; r2="${x##a*}"; r3=${x#"a*"}\n'
	printf 'echo "[$r1] [$r2] [$r3]"\n'
	printf 'p=\047a*\047\n'
	printf 'r1="${x##$p}"; r2=${x##"$p"}\n'
	printf 'echo "[$r1] [$r2]"\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'b b b b\n[aXb] [] [aXb]\n[] [aXb]\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
