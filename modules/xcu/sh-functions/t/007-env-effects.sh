# A function runs in the current environment: variables it sets stay, and
# shift in it changes only its own parameters.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-functions.007
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-functions.007
pc_test
unset PCV
pcf() { PCV=set; shift; echo "$#"; }
set a b
r=$(pcf x y z)
[ "$r" = 2 ] || pc_fail "inside the function \$# was '$r', expected 2 after shift"
pcf x y >/dev/null
[ "$PCV" = set ] || pc_fail "assignment in the function lost"
[ "$#" -eq 2 ] && [ "$1" = a ] || pc_fail "caller's parameters are '$*'"
pc_pass
