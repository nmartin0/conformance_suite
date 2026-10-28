# A for with no items does not run its body and has status zero; otherwise
# its status is that of the last body run. Without in, the list is "$@".
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-compound.005
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-compound.005
pc_test
empty=
r=x
(exit 9)
for i in $empty; do r=ran; done
rc=$?
[ "$r" = x ] && [ "$rc" -eq 0 ] || pc_fail "empty list: r='$r' status $rc"
(exit 9)
for i in; do r=ran; done
rc=$?
[ "$r" = x ] && [ "$rc" -eq 0 ] || pc_fail "for i in with nothing: r='$r' status $rc"
for i in 1 2; do (exit "$i"); done
rc=$?
[ "$rc" -eq 2 ] || pc_fail "status of the last body was $rc, expected 2"
for i in 1 2; do [ "$i" = 1 ]; done
rc=$?
[ "$rc" -eq 1 ] || pc_fail "status of the last body was $rc, expected 1"
set a 'b c' d
r=
for i; do r=$r[$i]; done
[ "$r" = '[a][b c][d]' ] || pc_fail "for i without in: '$r'"
pc_pass
