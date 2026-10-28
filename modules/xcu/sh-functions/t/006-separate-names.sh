# Functions and variables have separate name spaces.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-functions.006
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-functions.006
pc_test
pcn=var
pcn() { echo fn; }
[ "$pcn" = var ] || pc_fail "variable lost: '$pcn'"
r=$(pcn)
[ "$r" = fn ] || pc_fail "function lost: '$r'"
pc_pass
