# A single - operand is treated as the first operand and ignored.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.010
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.010
pc_test
r=$(printf 'echo "$1"\n' | ${PC_SH:-sh} -s - x)
[ "$r" = x ] || pc_fail "got '$r', expected x"
pc_pass
