# The special parameter 0 expands to the name of the script as the shell was
# given it (the command_file operand), or to the command_name operand of the
# -c form; the positional parameters follow in order.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.5-parameters.011
. "$PC_CORE/pc.sh"
pc_begin XCU.2.5-parameters.011
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'echo "$0 $1 $2 $#"\n'
	printf '"$PCSH" -c \047echo "$0 $1 $2"\047 nm x y\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh a b </dev/null >out 2>err
rc=$?
want=$(printf './s.sh a b 2\nnm x y\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
