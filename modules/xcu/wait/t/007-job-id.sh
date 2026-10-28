# wait %% waits for the current job (job control on).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wait.007
. "$PC_CORE/pc.sh"
pc_begin XCU.wait.007
set -m || pc_fail "set -m is not accepted"
pc_test
( sleep 1; : >wdone; exit 6 ) &
wait %%
rc=$?
[ -f wdone ] || pc_fail "wait returned before the job had finished"
[ "$rc" -eq 6 ] || pc_fail "wait exited $rc, wanted 6"
pc_pass
