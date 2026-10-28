# A word made of quoted and unquoted pieces, or ending in a substitution, is
# one token, and its quotes are removed only afterwards; a command name can
# therefore be written with quotes inside it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.3-token-recognition.006
. "$PC_CORE/pc.sh"
pc_begin XCU.2.3-token-recognition.006
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'ec"ho" one\n'
	printf '\047e\047cho two\n'
	printf 'e\\cho three\n'
	printf 'echo $(echo a; echo b)c\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'one\ntwo\nthree\na bc\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
