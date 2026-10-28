# A word in command-name position that names an alias is replaced by the
# alias value, which is read as new input (it can hold several commands); a
# word that is only an argument is not replaced; an alias value ending in a
# blank makes the next word eligible too.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.3-token-recognition.012
. "$PC_CORE/pc.sh"
pc_begin XCU.2.3-token-recognition.012
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'alias pcal1=\047echo X\047\n'
	printf 'pcal1 hi\n'
	printf 'alias pcal2=\047echo a; echo b\047\n'
	printf 'pcal2\n'
	printf 'alias pcal3=\047echo \047\n'
	printf 'alias pcal4=\047Y\047\n'
	printf 'pcal3 pcal4\n'
	printf 'alias pcal5=\047echo\047\n'
	printf 'pcal5 pcal4\n'
	printf 'echo pcal1\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'X hi\na\nb\nY\npcal4\npcal1\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
