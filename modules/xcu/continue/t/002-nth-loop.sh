# continue n returns to the top of the nth enclosing loop, abandoning the
# inner loops; continue 1 is the same as continue.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.continue.002
. "$PC_CORE/pc.sh"
pc_begin XCU.continue.002
pc_test
r=
for i in 1 2; do
	for j in a b c; do
		r=$r$i$j
		[ "$j" = b ] && continue 2
	done
	r=${r}X
done
[ "$r" = 1a1b2a2b ] || pc_fail "continue 2: got '$r', expected 1a1b2a2b"
r=
for i in 1 2; do
	for j in a b; do
		r=$r$i$j
		continue 1
		r=${r}X
	done
done
[ "$r" = 1a1b2a2b ] || pc_fail "continue 1: got '$r', expected 1a1b2a2b"
pc_pass
