# continue in a while or until loop repeats the condition list; in a for
# loop it takes the next item.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.continue.004
. "$PC_CORE/pc.sh"
pc_begin XCU.continue.004
pc_test
n=0
r=
while [ "$n" -lt 3 ]; do
	n=$((n + 1))
	continue
	r=${r}X
done
[ "$n" -eq 3 ] && [ -z "$r" ] || pc_fail "while: n=$n r='$r'"
n=0
until [ "$n" -ge 3 ]; do
	n=$((n + 1))
	continue
	r=${r}X
done
[ "$n" -eq 3 ] && [ -z "$r" ] || pc_fail "until: n=$n r='$r'"
for i in a b c; do
	r=$r$i
	continue
	r=${r}X
done
[ "$r" = abc ] || pc_fail "for: got '$r', expected abc"
[ "$i" = c ] || pc_fail "loop variable ends as '$i', expected c"
pc_pass
