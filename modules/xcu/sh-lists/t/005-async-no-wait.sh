# A command ended with & runs in the background: the shell goes on without
# waiting for it, and wait can collect it later.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-lists.005
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-lists.005
pc_test
{ sleep 1; : >pc.late; } &
pid=$!
[ -e pc.late ] && pc_fail "the shell waited for the asynchronous list"
wait "$pid"
[ -e pc.late ] || pc_fail "the asynchronous list did not complete"
pc_pass
