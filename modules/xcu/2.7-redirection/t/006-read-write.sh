# The <> operator opens the named file for reading and writing on standard
# input (or the descriptor written before it) and creates it when absent.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.7-redirection.006
. "$PC_CORE/pc.sh"
pc_begin XCU.2.7-redirection.006
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'printf \047%%s\\n\047 abc >f\n'
	printf 'cat <>f\n'
	printf ': <>g\n'
	printf 'if [ -f g ]; then echo created; else echo missing; fi\n'
	printf 'exec 3<>f\n'
	printf 'cat <&3\n'
	printf 'echo X >&3\n'
	printf 'exec 3>&-\n'
	printf 'cat f\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'abc\ncreated\nabc\nabc\nX\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
