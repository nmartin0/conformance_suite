# sh accepts input lines of any length: a command line of 100000 characters
# runs.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.019
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.019
printf 'echo ' >pc.long || pc_unresolved "write"
printf '%100000s' '' | tr ' ' a >>pc.long || pc_unresolved "tr"
printf '\n' >>pc.long || pc_unresolved "write"
pc_test
r=$(${PC_SH:-sh} pc.long </dev/null)
[ "${#r}" -eq 100000 ] || pc_fail "output has ${#r} characters, expected 100000"
pc_pass
