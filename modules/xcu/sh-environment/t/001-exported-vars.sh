# A utility gets exactly the exported variables, including ones set only for
# that command, and not the other shell variables.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-environment.001
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-environment.001
pc_test
export PCX1=e
PCX2=n
r=$(${PC_SH:-sh} -c 'printf "%s,%s,%s" "${PCX1-u}" "${PCX2-u}" "${PCX3-u}"')
[ "$r" = "e,u,u" ] || pc_fail "got '$r', expected e,u,u"
r=$(PCX3=t ${PC_SH:-sh} -c 'printf "%s,%s,%s" "${PCX1-u}" "${PCX2-u}" "${PCX3-u}"')
[ "$r" = "e,u,t" ] || pc_fail "with a temporary assignment got '$r', expected e,u,t"
pc_pass
