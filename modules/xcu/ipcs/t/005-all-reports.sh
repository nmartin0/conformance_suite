# All three reports without -q, -m, -s.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ipcs.005
. "$PC_CORE/pc.sh"
pc_begin XCU.ipcs.005
LC_ALL=C; export LC_ALL
pc_test
for o in "" "-a" "-b" "-c" "-o" "-p" "-t" "-bcopt"; do
	ipcs $o >out 2>err </dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "ipcs $o exited with status $rc"
	n=0 q=no m=no s=no
	while IFS= read -r l; do
		n=$((n + 1))
		[ "$n" -eq 1 ] && continue
		case $l in *"Message Queue"*) q=yes ;; esac
		case $l in *"Shared Memory"*) m=yes ;; esac
		case $l in *Semaphore*) s=yes ;; esac
	done <out
	[ "$q" = yes ] && [ "$m" = yes ] && [ "$s" = yes ] || pc_fail "ipcs $o did not write all three reports (queues $q, memory $m, semaphores $s)"
done
ipcs -q -a >out 2>err </dev/null || pc_fail "ipcs -q -a failed"
n=0
while IFS= read -r l; do
	n=$((n + 1))
	[ "$n" -eq 1 ] && continue
	case $l in *"Shared Memory"* | *Semaphore*) pc_fail "ipcs -q -a wrote the line '$l' of another report" ;; esac
done <out
pc_pass
