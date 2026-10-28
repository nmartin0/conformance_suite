# A non-interactive shell does not do filename expansion on the word of a
# redirection, so a pattern is used as the file name as written.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.7-redirection.011
. "$PC_CORE/pc.sh"
pc_begin XCU.2.7-redirection.011
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf ': >pcr1\n'
	printf ': >pcr2\n'
	printf 'echo x >pcr*\n'
	printf 'if [ -f \047pcr*\047 ]; then echo literal; else echo no-literal; fi\n'
	printf 'if [ -s pcr1 ]; then echo touched; else echo untouched; fi\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'literal\nuntouched\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
