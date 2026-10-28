# while runs its body as long as the condition succeeds, until as long as it
# fails.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-compound.012
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-compound.012
pc_test
n=0
r=
while [ "$n" -lt 3 ]; do n=$((n + 1)); r=$r$n; done
[ "$r" = 123 ] || pc_fail "while: '$r'"
n=0
r=
until [ "$n" -ge 3 ]; do n=$((n + 1)); r=$r$n; done
[ "$r" = 123 ] || pc_fail "until: '$r'"
r=x
while false; do r=ran; done
[ "$r" = x ] || pc_fail "while false ran its body"
r=x
until true; do r=ran; done
[ "$r" = x ] || pc_fail "until true ran its body"
pc_pass
