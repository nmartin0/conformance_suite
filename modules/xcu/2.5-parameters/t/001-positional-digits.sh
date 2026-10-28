# Positional parameters are named by decimal numbers, leading zeros do not
# make them octal, and a number of more than one digit needs braces: without
# braces only the first digit belongs to the name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.5-parameters.001
. "$PC_CORE/pc.sh"
pc_begin XCU.2.5-parameters.001
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'set -- A B C D E F G H I J K\n'
	printf 'echo "$1$2" "${10}" "${11}" "$10" "${08}" "${008}" "$8" "${1}0"\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'AB J K A0 H H H A0\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
