# Command status values: 127 when the command is not found, 126 when found
# but not executable; for a command ended by a signal, above 128; otherwise
# the exit value.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-errors.012
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-errors.012
printf '%s\n' ':' >pc.ne || pc_unresolved "write"
chmod 644 pc.ne || pc_unresolved "chmod"
printf '%s\n' 'exit 33' >pc.ex || pc_unresolved "write"
chmod 755 pc.ex || pc_unresolved "chmod"
pc_test
(pc-no-such-utility-xyz) 2>/dev/null
[ "$?" -eq 127 ] || pc_fail "not found: $?"
(./pc.ne) 2>/dev/null
[ "$?" -eq 126 ] || pc_fail "not executable: $?"
./pc.ex
[ "$?" -eq 33 ] || pc_fail "exit value: $?"
${PC_SH:-sh} -c 'kill -s KILL $$' </dev/null
[ "$?" -gt 128 ] || pc_fail "signal: $?"
pc_pass
