# The constructed command runs in the current shell environment: variables,
# functions and positional parameters it sets stay set.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.eval.002
. "$PC_CORE/pc.sh"
pc_begin XCU.eval.002
pc_test
unset pcv
eval 'pcv=5'
[ "$pcv" = 5 ] || pc_fail "variable not set (pcv='$pcv')"
eval 'pcf() { echo fn; }'
r=$(pcf)
[ "$r" = fn ] || pc_fail "function not defined (got '$r')"
eval 'set -- a b c'
[ "$#" -eq 3 ] && [ "$2" = b ] || pc_fail "positional parameters not set"
pc_pass
