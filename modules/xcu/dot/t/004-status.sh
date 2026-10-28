# The status of the dot utility is that of the last command executed in the
# file, and zero when the file runs no command.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dot.004
. "$PC_CORE/pc.sh"
pc_begin XCU.dot.004
printf '%s\n' '(exit 5)' >pc.d4 || pc_unresolved "write"
printf '%s\n' 'true' >pc.d5 || pc_unresolved "write"
: >pc.d6 || pc_unresolved "write"
printf '%s\n' '# only a comment' '' >pc.d7 || pc_unresolved "write"
pc_test
. ./pc.d4
rc=$?
[ "$rc" -eq 5 ] || pc_fail "file ending with (exit 5) gave $rc"
false
. ./pc.d5
rc=$?
[ "$rc" -eq 0 ] || pc_fail "file ending with true gave $rc"
(exit 3)
. ./pc.d6
rc=$?
[ "$rc" -eq 0 ] || pc_fail "empty file gave $rc"
(exit 3)
. ./pc.d7
rc=$?
[ "$rc" -eq 0 ] || pc_fail "file with only a comment gave $rc"
pc_pass
