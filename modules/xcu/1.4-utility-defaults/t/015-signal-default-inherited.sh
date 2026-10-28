# A utility whose ASYNCHRONOUS EVENTS section says Default and that starts with SIGTERM or SIGHUP at the default action is terminated by that signal: sleep, and cat and sort blocked on a FIFO, report a signal-termination status
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.015
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.015
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkfifo f1 f2 || pc_unresolved "cannot make a FIFO"
probe=$(${PC_SH:-sh} -c 'kill -TERM $$; sleep 1; echo survived' 2>/dev/null)
[ "$probe" = survived ] && pc_unresolved "SIGTERM is ignored at the start of this test"
probe=$(${PC_SH:-sh} -c 'kill -HUP $$; sleep 1; echo survived' 2>/dev/null)
[ "$probe" = survived ] && pc_unresolved "SIGHUP is ignored at the start of this test"
pids=
cleanup() { for p in $pids; do kill -KILL "$p" 2>/dev/null; done; }
trap 'cleanup; cd "$pc_top" && rm -rf "$pc_wd"' EXIT
pc_test
for sig in TERM HUP; do
	for c in "sleep 30" "cat f1" "sort f2"; do
		(exec $c) </dev/null >/dev/null 2>&1 &
		p=$!
		pids="$pids $p"
		sleep 1
		kill -s "$sig" "$p"
		wait "$p"
		rc=$?
		[ "$rc" -gt 128 ] || pc_fail "$c after SIG$sig: exit status $rc is not a signal status"
		name=$(kill -l "$rc")
		[ "$name" = "$sig" ] || pc_fail "$c after SIG$sig: the status $rc names '$name'"
	done
done
pc_pass
