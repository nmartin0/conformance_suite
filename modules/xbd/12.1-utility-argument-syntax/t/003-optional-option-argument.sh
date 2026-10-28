# An option with an optional option-argument, given alone, does not take the next argument.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.12.1-utility-argument-syntax.003
#
# pr -e[char][gap] and pr -n[char][width] have optional option-arguments.
# Given alone they use the defaults (tab character, gap 8; tab separator,
# width 5) and FILE stays a file operand. If FILE were taken as the
# option-argument there would be no input file and the (empty) standard
# input would be read.
. "$PC_CORE/pc.sh"
pc_begin XBD.12.1-utility-argument-syntax.003
LC_ALL=POSIX
export LC_ALL
TAB=$(printf '\t')
SP=' '
printf 'a\tb\n' >FILE || pc_unresolved "cannot create FILE"
printf 'x\n' >X || pc_unresolved "cannot create X"
pc_test
got=$(pr -t -e FILE 2>/dev/null </dev/null)
[ "$got" = "a       b" ] || pc_fail "pr -t -e FILE gave '$got', expected a, seven spaces and b"
got=$(pr -t -n X 2>/dev/null </dev/null)
# The number fills the first five columns (its alignment and padding are not stated),
# then the tab, then the text.
case $got in
*"$TAB"x) ;;
*) pc_fail "pr -t -n X gave '$got', expected a five-column number, a tab and x" ;;
esac
num=${got%"$TAB"x}
[ "${#num}" -eq 5 ] || pc_fail "pr -t -n X numbered in '$num', not five columns wide"
# Spaces or zeros may fill the field (left or right); the value must be 1.
rest=$num
while :; do
	case $rest in
	" "* | 0*) rest=${rest#?} ;;
	*) break ;;
	esac
done
rest2=${rest%" "}
while [ "$rest2" != "$rest" ]; do
	rest=$rest2
	rest2=${rest%" "}
done
[ "$rest" = 1 ] || pc_fail "pr -t -n X numbered the line '$num', expected the number 1"
pc_pass
