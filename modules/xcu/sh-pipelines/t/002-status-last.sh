# The status of a pipeline is the status of its last command.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-pipelines.002
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-pipelines.002
pc_test
false | true
rc=$?
[ "$rc" -eq 0 ] || pc_fail "false | true gave $rc"
true | false
rc=$?
[ "$rc" -eq 1 ] || pc_fail "true | false gave $rc"
true | (exit 3)
rc=$?
[ "$rc" -eq 3 ] || pc_fail "true | (exit 3) gave $rc"
(exit 4) | (exit 3)
rc=$?
[ "$rc" -eq 3 ] || pc_fail "(exit 4) | (exit 3) gave $rc"
pc_pass
