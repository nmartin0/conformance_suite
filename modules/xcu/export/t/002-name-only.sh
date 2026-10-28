# export name gives an existing variable the export attribute, and later
# assignments to it are still passed on; a variable that was unset when
# exported is passed on once it is assigned.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.export.002
. "$PC_CORE/pc.sh"
pc_begin XCU.export.002
pc_test
PCE3=one
export PCE3
r=$(${PC_SH:-sh} -c 'printf %s "${PCE3-unset}"')
[ "$r" = one ] || pc_fail "got '$r', expected one"
PCE3=two
r=$(${PC_SH:-sh} -c 'printf %s "${PCE3-unset}"')
[ "$r" = two ] || pc_fail "after reassignment got '$r', expected two"
export PCE4
r=$(${PC_SH:-sh} -c 'printf %s "${PCE4-unset}"')
[ "$r" = unset ] || pc_fail "an unset variable appeared in the environment as '$r'"
PCE4=late
r=$(${PC_SH:-sh} -c 'printf %s "${PCE4-unset}"')
[ "$r" = late ] || pc_fail "assigned after export: got '$r', expected late"
pc_pass
