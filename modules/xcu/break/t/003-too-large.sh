# When n is greater than the number of enclosing loops, break leaves the
# outermost enclosing loop.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.break.003
. "$PC_CORE/pc.sh"
pc_begin XCU.break.003
pc_test
r=
for i in 1 2; do
	for j in a b; do
		r=$r$i$j
		break 9
	done
done
r=${r}x
[ "$r" = 1ax ] || pc_fail "got '$r', expected 1ax"
pc_pass
