# The getopts utility parses a mixed command line as the guidelines say.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.12.2-utility-syntax-guidelines.012
#
# -a -b are flags, -c and -d take an option-argument. The line holds a group
# (-ab), an option-argument in the next argument (-c val1), one attached
# (-dval2), a -- and then arguments that begin with a hyphen-minus.
. "$PC_CORE/pc.sh"
pc_begin XBD.12.2-utility-syntax-guidelines.012
pc_test
set -- -ab -c val1 -dval2 -- -a rest
OPTIND=1
seq=
while getopts abc:d: o; do
	case $o in
	a | b) seq="$seq $o" ;;
	c | d) seq="$seq $o=$OPTARG" ;;
	*) seq="$seq ?" ;;
	esac
done
[ "$seq" = " a b c=val1 d=val2" ] || pc_fail "getopts abc:d: on -ab -c val1 -dval2 -- -a rest gave '$seq'"
shift $((OPTIND - 1))
[ "$#" -eq 2 ] && [ "$1" = -a ] && [ "$2" = rest ] || pc_fail "after the options the operands were '$*', expected -a rest"
set -- -a first -b
OPTIND=1
seq=
while getopts ab o; do
	seq="$seq $o"
done
[ "$seq" = " a" ] || pc_fail "getopts ab on -a first -b gave '$seq', expected only a (the first operand ends the options)"
[ "$OPTIND" -eq 2 ] || pc_fail "OPTIND is $OPTIND after the first operand, expected 2"
pc_pass
