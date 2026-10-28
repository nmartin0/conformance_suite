# Unsetting an exported variable removes it from the environment of later
# utilities, and a later assignment does not bring it back.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unset.006
. "$PC_CORE/pc.sh"
pc_begin XCU.unset.006
pc_test
export PCU6=1
unset PCU6
r=$(${PC_SH:-sh} -c 'printf %s "${PCU6-unset}"')
[ "$r" = unset ] || pc_fail "still in the environment: '$r'"
PCU6=2
r=$(${PC_SH:-sh} -c 'printf %s "${PCU6-unset}"')
[ "$r" = unset ] || pc_fail "export attribute survived the unset: '$r'"
pc_pass
