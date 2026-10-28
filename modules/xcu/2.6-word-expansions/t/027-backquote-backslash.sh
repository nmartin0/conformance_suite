# In the backquote form, a backslash keeps its literal meaning except before
# a dollar sign, backquote or backslash, where it is removed; inner
# backquotes are nested by escaping them with backslashes.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.027
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.027
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'x=`printf %%s \047\\\\\047`\n'
	printf 'if [ "$x" = \047\\\047 ]; then echo ok1; else echo bad1; fi\n'
	printf 'x=`printf %%s \047\\n\047`\n'
	printf 'if [ "$x" = \047\\n\047 ]; then echo ok2; else echo bad2; fi\n'
	printf 'x=`printf %%s \047\\$\047`\n'
	printf 'if [ "$x" = \047$\047 ]; then echo ok3; else echo bad3; fi\n'
	printf 'x=`echo \\`echo in\\``\n'
	printf 'if [ "$x" = in ]; then echo ok4; else echo bad4; fi\n'
	printf 'y=out\n'
	printf 'x=`echo \\$y`\n'
	printf 'if [ "$x" = out ]; then echo ok5; else echo bad5; fi\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'ok1\nok2\nok3\nok4\nok5\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
