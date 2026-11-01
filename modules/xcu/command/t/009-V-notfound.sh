# command -V for an unknown name fails.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.009
# MUTANT    command.Vnf_status0
# MUTANT    command.Vnf_status0_msg
# VARIANT   command.Vnf_msg_stdout
# VARIANT   command.Vnf_msg_stderr_127
#
# What is written, and where, is unspecified; only the status is required.
. "$PC_CORE/pc.sh"
pc_begin XCU.command.009
pc_test
command -V pcnosuch6 >/dev/null 2>&1
rc=$?
[ "$rc" -gt 0 ] || pc_fail "command -V exited $rc for an unknown name"
pc_pass
