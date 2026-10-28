# The status of while and until is that of the last body run, or zero if the
# body never ran; $? in the body is the status of the condition.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-compound.013
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-compound.013
pc_test
(exit 9)
while false; do :; done
[ "$?" -eq 0 ] || pc_fail "while with no pass: $?"
(exit 9)
until true; do :; done
[ "$?" -eq 0 ] || pc_fail "until with no pass: $?"
n=0
while [ "$n" -lt 2 ]; do n=$((n + 1)); (exit 4); done
[ "$?" -eq 4 ] || pc_fail "while: $?"
n=0
until [ "$n" -ge 2 ]; do n=$((n + 1)); (exit 6); done
[ "$?" -eq 6 ] || pc_fail "until: $?"
n=0
while [ "$n" -lt 1 ]; do r=$?; n=1; done
[ "$r" = 0 ] || pc_fail "\$? in a while body was '$r', expected 0"
n=0
until [ "$n" -ge 1 ]; do r=$?; n=1; done
[ "$r" = 1 ] || pc_fail "\$? in an until body was '$r', expected 1"
pc_pass
