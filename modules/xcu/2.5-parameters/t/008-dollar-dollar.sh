# The special parameter $ expands to the decimal process ID of the shell,
# written without leading zeros and nonzero, and a subshell or command
# substitution sees the same value.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.5-parameters.008
. "$PC_CORE/pc.sh"
pc_begin XCU.2.5-parameters.008
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'case $$ in \047\047|*[!0-9]*|0*) echo bad;; *) echo ok;; esac\n'
	printf 'a=$$\n'
	printf 'b=$(echo $$)\n'
	printf 'c=$( (echo $$) )\n'
	printf 'if [ "$a" = "$b" ] && [ "$a" = "$c" ]; then echo same; else echo different; fi\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'ok\nsame\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
