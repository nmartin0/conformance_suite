# Without an operand, break leaves the innermost for, while or until loop
# and execution continues after that loop.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.break.001
. "$PC_CORE/pc.sh"
pc_begin XCU.break.001
pc_test
r=
for i in 1 2 3; do
	r=$r$i
	if [ "$i" = 2 ]; then break; fi
done
r=${r}x
[ "$r" = 12x ] || pc_fail "for loop: got '$r', expected 12x"
r=
n=0
while [ "$n" -lt 5 ]; do
	n=$((n + 1))
	r=$r$n
	[ "$n" -eq 3 ] && break
done
r=${r}x
[ "$r" = 123x ] || pc_fail "while loop: got '$r', expected 123x"
r=
n=0
until [ "$n" -ge 5 ]; do
	n=$((n + 1))
	r=$r$n
	[ "$n" -eq 2 ] && break
done
r=${r}x
[ "$r" = 12x ] || pc_fail "until loop: got '$r', expected 12x"
pc_pass
