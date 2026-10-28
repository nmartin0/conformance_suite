# && and || have equal precedence and group from the left.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-lists.001
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-lists.001
pc_test
r=$(false && echo foo || echo bar)
[ "$r" = bar ] || pc_fail "false && echo foo || echo bar gave '$r'"
r=$(true || echo foo && echo bar)
[ "$r" = bar ] || pc_fail "true || echo foo && echo bar gave '$r'"
r=$(false || false || echo x)
[ "$r" = x ] || pc_fail "false || false || echo x gave '$r'"
r=$(true && echo a && echo b)
[ "$r" = "a
b" ] || pc_fail "true && echo a && echo b gave '$r'"
pc_pass
