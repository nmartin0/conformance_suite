# Arithmetic expansion has the conditional operator, takes decimal, octal
# (leading 0) and hexadecimal (0x) constants, reads variables by name, and
# expands parameters, command substitutions and blanks inside the
# expression.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.032
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.032
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'echo $((1?2:3)) $((0?2:3)) $((1?0?4:5:6))\n'
	printf 'echo $((010)) $((0x1F)) $((0X1f)) $((10))\n'
	printf 'x=5\n'
	printf 'echo $((x+1)) $((x*x)) $(( x )) $(( 2 + 3 ))\n'
	printf 'y=4; echo $(($y+1)) $(( $y * 2 ))\n'
	printf 'echo $(( $(echo 6) / 2 ))\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '2 3 5\n8 31 31 10\n6 25 5 5\n5 8\n3\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
