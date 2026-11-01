# The command name given to command is not a reserved word.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.013
# MUTANT    command.reserved_recognized
# MUTANT    command.reserved_status0
. "$PC_CORE/pc.sh"
pc_begin XCU.command.013
pc_test
for w in if then else fi while do done case esac '!' '{' '}'; do
	command "$w" >/dev/null 2>&1
	rc=$?
	[ "$rc" -eq 127 ] || pc_fail "command $w exited $rc, wanted 127 (no utility named $w)"
done
pc_pass
