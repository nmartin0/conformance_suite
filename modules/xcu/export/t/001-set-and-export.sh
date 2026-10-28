# export name=word sets the variable and gives it the export attribute, so
# utilities run later see it; a plain assignment does not export.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.export.001
. "$PC_CORE/pc.sh"
pc_begin XCU.export.001
pc_test
export PCE1=val
[ "$PCE1" = val ] || pc_fail "export name=word did not set the value (PCE1='$PCE1')"
r=$(${PC_SH:-sh} -c 'printf %s "${PCE1-unset}"')
[ "$r" = val ] || pc_fail "a later utility saw PCE1='$r', expected val"
PCE2=plain
r=$(${PC_SH:-sh} -c 'printf %s "${PCE2-unset}"')
[ "$r" = unset ] || pc_fail "an unexported variable reached a utility (PCE2='$r')"
pc_pass
