# Without an operand, continue skips the rest of the body of the innermost
# for, while or until loop and starts the next iteration.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.continue.001
. "$PC_CORE/pc.sh"
pc_begin XCU.continue.001
pc_test
r=
for i in 1 2 3; do
	if [ "$i" = 2 ]; then continue; fi
	r=$r$i
done
[ "$r" = 13 ] || pc_fail "for loop: got '$r', expected 13"
r=
n=0
while [ "$n" -lt 4 ]; do
	n=$((n + 1))
	[ "$n" -eq 2 ] && continue
	r=$r$n
done
[ "$r" = 134 ] || pc_fail "while loop: got '$r', expected 134"
r=
n=0
until [ "$n" -ge 4 ]; do
	n=$((n + 1))
	[ "$n" -eq 3 ] && continue
	r=$r$n
done
[ "$r" = 124 ] || pc_fail "until loop: got '$r', expected 124"
pc_pass
