# command -v for an unknown name writes nothing and fails.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.007
# MUTANT    command.nf_prints_name
# MUTANT    command.nf_status0
# MUTANT    command.nf_msg_stdout
# VARIANT   command.nf_diag_127
# VARIANT   command.nf_status2
#
# The page says no output; the standard error stream may carry a
# diagnostic, and any status above zero will do.
. "$PC_CORE/pc.sh"
pc_begin XCU.command.007
pc_test
command -v pcnosuch5 >out 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "command -v exited $rc for an unknown name"
pc_need_file out "the output of command -v"
[ ! -s out ] || pc_fail "command -v wrote to standard output for an unknown name"
pc_pass
