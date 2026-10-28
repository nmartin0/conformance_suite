# ipcs -q writes only its own report.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ipcs.002
. "$PC_CORE/pc.sh"
pc_begin XCU.ipcs.002
LC_ALL=C; export LC_ALL
pc_test
ipcs -q >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ipcs -q exited with status $rc"
n=0 has=no
while IFS= read -r l; do
	n=$((n + 1))
	[ "$n" -eq 1 ] && continue
	case $l in
	*"Message Queue"*) has=yes ;;
	esac
	case $l in
	*"Shared Memory"* | *"Semaphore"*) pc_fail "ipcs -q wrote the line '$l' of another report" ;;
	esac
done <out
[ "$has" = yes ] || pc_fail "ipcs -q did not write the Message Queue report"
pc_pass
