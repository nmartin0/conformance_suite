# Without -f or -v, unset treats the name as a variable: the variable goes
# and a function of the same name stays.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unset.003
. "$PC_CORE/pc.sh"
pc_begin XCU.unset.003
pc_test
pcf() { echo fn; }
pcf=var
unset pcf
[ -z "${pcf+set}" ] || pc_fail "the variable is still set"
r=$(pcf)
[ "$r" = fn ] || pc_fail "the function was removed (got '$r')"
pc_pass
