# The commands of a command substitution run in a subshell environment: a
# variable they set or a directory they change to does not affect the
# current shell.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.024
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.024
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'x=1\n'
	printf 'y=$(x=2; echo $x)\n'
	printf 'echo $x $y\n'
	printf 'p=$PWD\n'
	printf ': $(cd /)\n'
	printf 'if [ "$PWD" = "$p" ]; then echo same; else echo moved; fi\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '1 2\nsame\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
