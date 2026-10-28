# Lists separated by ; or newline run one after the other, and the status is
# that of the last one run.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-lists.003
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-lists.003
pc_test
r=; r=${r}a; r=${r}b
[ "$r" = ab ] || pc_fail "order: '$r'"
false; true
[ "$?" -eq 0 ] || pc_fail "false; true"
true; false
[ "$?" -eq 1 ] || pc_fail "true; false"
pc_pass
