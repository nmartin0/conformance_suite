# ipcs -m writes only its own report.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ipcs.003
. "$PC_CORE/pc.sh"
pc_begin XCU.ipcs.003
LC_ALL=C; export LC_ALL
pc_test
ipcs -m >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ipcs -m exited with status $rc"
n=0 has=no
while IFS= read -r l; do
	n=$((n + 1))
	[ "$n" -eq 1 ] && continue
	case $l in
	*"Shared Memory"*) has=yes ;;
	esac
	case $l in
	*"Message Queue"* | *"Semaphore"*) pc_fail "ipcs -m wrote the line '$l' of another report" ;;
	esac
done <out
[ "$has" = yes ] || pc_fail "ipcs -m did not write the Shared Memory report"
pc_pass
