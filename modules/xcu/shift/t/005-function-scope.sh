# Inside a function, shift acts on the function's own positional parameters;
# the caller's are restored when the function returns.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.shift.005
. "$PC_CORE/pc.sh"
pc_begin XCU.shift.005
pc_test
pcf() { shift; echo "$1"; }
set a b
r=$(pcf x y)
[ "$r" = y ] || pc_fail "function saw '$r', expected y"
pcf x y >/dev/null
[ "$#" -eq 2 ] && [ "$1" = a ] || pc_fail "caller's parameters changed to '$*'"
pc_pass
