# The status of if is that of the then or else list that ran, or zero when
# none ran; the status of a condition is $? at the start of the next list.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-compound.011
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-compound.011
pc_test
(exit 9)
if false; then :; fi
[ "$?" -eq 0 ] || pc_fail "no list ran: $?"
if true; then (exit 4); fi
[ "$?" -eq 4 ] || pc_fail "then list: $?"
if false; then :; else (exit 5); fi
[ "$?" -eq 5 ] || pc_fail "else list: $?"
if (exit 3); then :; else r=$?; fi
[ "$r" = 3 ] || pc_fail "\$? in the else list was '$r', expected 3"
if false; then :; elif (exit 6); then :; else r=$?; fi
[ "$r" = 6 ] || pc_fail "\$? after a failed elif was '$r', expected 6"
pc_pass
