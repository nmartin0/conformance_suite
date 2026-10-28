# The dot utility executes the commands of the file in the current shell
# environment: variables, functions and positional parameters it sets stay
# set.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dot.001
. "$PC_CORE/pc.sh"
pc_begin XCU.dot.001
printf '%s\n' 'pcv=dotted' 'pcdf() { echo fnbody; }' 'set -- p q' >pc.d1 || pc_unresolved "cannot write pc.d1"
unset pcv
pc_test
. ./pc.d1
[ "$pcv" = dotted ] || pc_fail "variable not set (pcv='$pcv')"
r=$(pcdf)
[ "$r" = fnbody ] || pc_fail "function not defined (got '$r')"
[ "$#" -eq 2 ] && [ "$1" = p ] || pc_fail "set -- in the file did not change the positional parameters"
pc_pass
