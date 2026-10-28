# An unquoted hash that begins a word starts a comment that runs to the end
# of the line, without removing the newline; a hash inside a word, or a
# quoted or escaped one, is an ordinary character.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.3-token-recognition.004
. "$PC_CORE/pc.sh"
pc_begin XCU.2.3-token-recognition.004
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'echo a # echo b\n'
	printf '# echo c\n'
	printf 'echo d#e\n'
	printf 'echo f \047#g\047 "#h" \\#i\n'
	printf 'echo j;# echo k\n'
	printf 'echo l\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'a\nd#e\nf #g #h #i\nj\nl\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
