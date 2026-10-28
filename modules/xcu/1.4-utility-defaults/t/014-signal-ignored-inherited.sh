# A utility whose ASYNCHRONOUS EVENTS section says Default and that starts with SIGTERM or SIGHUP ignored keeps ignoring it: sleep, and cat and sort blocked on a FIFO, survive the signal
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.014
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.014
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkfifo f1 f2 || pc_unresolved "cannot make a FIFO"
probe=$( (trap '' TERM; ${PC_SH:-sh} -c 'kill -TERM $$; sleep 1; echo ign') 2>/dev/null )
[ "$probe" = ign ] || pc_unresolved "SIGTERM cannot be ignored for a child here"
pids=
cleanup() { for p in $pids; do kill -KILL "$p" 2>/dev/null; done; }
trap 'cleanup; cd "$pc_top" && rm -rf "$pc_wd"' EXIT
pc_test
(trap '' TERM HUP; exec sleep 30) </dev/null >/dev/null 2>&1 &
pids="$pids $!"
(trap '' TERM HUP; exec cat f1) </dev/null >/dev/null 2>&1 &
pids="$pids $!"
(trap '' TERM HUP; exec sort f2) </dev/null >/dev/null 2>&1 &
pids="$pids $!"
sleep 1
for p in $pids; do kill -s TERM "$p"; kill -s HUP "$p"; done
sleep 1
for p in $pids; do
	kill -0 "$p" 2>/dev/null || pc_fail "process $p did not keep ignoring SIGTERM and SIGHUP"
done
pc_pass
