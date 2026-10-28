# The shell takes PWD from the environment only when it is an absolute path
# of the current directory without dot or dot-dot components; otherwise (not
# set, wrong directory, dot components) it sets PWD to the physical path
# that pwd -P gives.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.5-parameters.017
. "$PC_CORE/pc.sh"
pc_begin XCU.2.5-parameters.017
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'p=$(pwd -P)\n'
	printf 'env PWD=/ "$PCSH" -c \047[ "$PWD" = "$(pwd -P)" ] && echo a || echo A\047\n'
	printf 'env "PWD=$p/." "$PCSH" -c \047[ "$PWD" = "$(pwd -P)" ] && echo b || echo B\047\n'
	printf '(unset PWD; exec "$PCSH" -c \047[ "$PWD" = "$(pwd -P)" ] && echo c || echo C\047)\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'a\nb\nc\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
