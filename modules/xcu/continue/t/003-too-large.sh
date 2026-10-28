# When n is greater than the number of enclosing loops, continue applies to
# the outermost enclosing loop.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.continue.003
. "$PC_CORE/pc.sh"
pc_begin XCU.continue.003
pc_test
r=
for i in 1 2; do
	for j in a b; do
		r=$r$i$j
		continue 9
	done
	r=${r}X
done
[ "$r" = 1a2a ] || pc_fail "got '$r', expected 1a2a"
pc_pass
