# A function's operands become its positional parameters and $#; $0 is
# unchanged; the caller's parameters come back afterwards.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-functions.002
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-functions.002
pc_test
set a b
pcf() { echo "$# $1 $2 $0"; }
r=$(pcf x y z)
[ "$r" = "3 x y $0" ] || pc_fail "got '$r', expected '3 x y $0'"
r=$(pcf)
[ "$r" = "0   $0" ] || pc_fail "with no operands got '$r'"
pcf p q >/dev/null
[ "$#" -eq 2 ] && [ "$1" = a ] && [ "$2" = b ] || pc_fail "caller's parameters are '$*'"
pc_pass
