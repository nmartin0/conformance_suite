# A break written in the condition list of a while or until loop leaves that
# loop.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.break.004
. "$PC_CORE/pc.sh"
pc_begin XCU.break.004
pc_test
r=
while break; do r=${r}body; done
r=${r}after
[ "$r" = after ] || pc_fail "while: got '$r', expected after"
r=
until break; do r=${r}body; done
r=${r}after
[ "$r" = after ] || pc_fail "until: got '$r', expected after"
pc_pass
