# The command name given to command is not a reserved word.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.013
. "$PC_CORE/pc.sh"
pc_begin XCU.command.013
pc_test
command if >/dev/null 2>&1
rc=$?
[ "$rc" -eq 127 ] || pc_fail "command if exited $rc, wanted 127 (no utility named if)"
pc_pass
