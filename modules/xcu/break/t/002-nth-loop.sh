# break n leaves the nth enclosing loop; break 1 is the same as break.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.break.002
. "$PC_CORE/pc.sh"
pc_begin XCU.break.002
pc_test
r=
for i in 1 2; do
	for j in a b c; do
		r=$r$i$j
		[ "$j" = b ] && break 2
	done
	r=${r}X
done
[ "$r" = 1a1b ] || pc_fail "break 2: got '$r', expected 1a1b"
r=
for i in 1 2; do
	for j in a b c; do
		r=$r$i$j
		break 1
	done
done
[ "$r" = 1a2a ] || pc_fail "break 1: got '$r', expected 1a2a"
r=
for i in 1 2; do
	for j in a b; do
		for k in x y; do
			r=$r$i$j$k
			break 3
		done
	done
done
[ "$r" = 1ax ] || pc_fail "break 3: got '$r', expected 1ax"
pc_pass
