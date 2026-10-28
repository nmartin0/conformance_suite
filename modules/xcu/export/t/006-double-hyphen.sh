# export accepts "--" as the end of options.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.export.006
. "$PC_CORE/pc.sh"
pc_begin XCU.export.006
pc_test
export -- PCE8=dd
[ "$PCE8" = dd ] || pc_fail "PCE8='$PCE8'"
r=$(${PC_SH:-sh} -c 'printf %s "${PCE8-unset}"')
[ "$r" = dd ] || pc_fail "not exported: '$r'"
pc_pass
