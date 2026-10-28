# kill %% signals the current job (job control on).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.011
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.011
p=
trap 'kill -s KILL "$p" 2>/dev/null' EXIT
set -m || pc_fail "set -m is not accepted"
pc_test
sleep 100 &
p=$!
kill '%%' 2>/dev/null || pc_fail "kill %% failed"
wait "$p"
st=$?
n=$(kill -l "$st")
[ "$n" = TERM ] || pc_fail "the job ended by '$n' after kill %%, wanted TERM"
pc_pass
