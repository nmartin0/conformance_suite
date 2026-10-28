# sh -c with no command_name sets $0 to the first argument the shell was
# started with, which for a command found by search is the command name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.002
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.002
pc_test
r=$(${PC_SH:-sh} -c 'printf %s "$0"' </dev/null)
[ "$r" = "${PC_SH:-sh}" ] || pc_fail "got '$r', expected ${PC_SH:-sh} (the name the shell was run by)"
pc_pass
