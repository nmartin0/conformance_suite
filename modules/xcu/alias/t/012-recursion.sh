# An alias is not substituted again inside its own value.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.012
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.012
pcl5() { echo "ran $*"; }
pc_test
alias pcl5='pcl5 arg'
got=$(eval pcl5)
[ "$got" = "ran arg" ] || pc_fail "got '$got', wanted 'ran arg'"
pc_pass
