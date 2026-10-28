# sh -c runs the command string; command_name becomes $0 and the later
# operands $1, $2 ...; a string of several lines runs them in order.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.001
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.001
pc_test
r=$(${PC_SH:-sh} -c 'echo "$0|$1|$2"' pcname a b </dev/null)
[ "$r" = "pcname|a|b" ] || pc_fail "got '$r', expected pcname|a|b"
r=$(${PC_SH:-sh} -c 'echo one
echo two' </dev/null)
nl='
'
[ "$r" = "one${nl}two" ] || pc_fail "two-line string gave '$r'"
pc_pass
