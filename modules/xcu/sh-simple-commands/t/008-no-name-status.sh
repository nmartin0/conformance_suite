# A simple command with no command name has status zero, or the status of
# the last command substitution it ran.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-simple-commands.008
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-simple-commands.008
pc_test
false
PCV=1
rc=$?
[ "$rc" -eq 0 ] || pc_fail "plain assignment after false gave $rc"
PCV=$(exit 3)
rc=$?
[ "$rc" -eq 3 ] || pc_fail "assignment of (exit 3) gave $rc"
PCV=$(exit 3)$(exit 5)
rc=$?
[ "$rc" -eq 5 ] || pc_fail "two substitutions gave $rc, expected 5"
false
>pc.n1
rc=$?
[ "$rc" -eq 0 ] || pc_fail "a bare redirection after false gave $rc"
pc_pass
