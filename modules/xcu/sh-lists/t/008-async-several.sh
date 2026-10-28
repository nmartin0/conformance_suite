# Several asynchronous lists in one line all run; wait with no operand waits
# for all of them.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-lists.008
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-lists.008
pc_test
{ : >pc.a1; } & { : >pc.a2; } & { sleep 1; : >pc.a3; } &
wait
[ -e pc.a1 ] && [ -e pc.a2 ] && [ -e pc.a3 ] || pc_fail "one of the asynchronous lists did not run"
pc_pass
