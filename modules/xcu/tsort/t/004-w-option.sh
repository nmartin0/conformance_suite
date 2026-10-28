# With -w the exit status is the number of cycles found, and a message about them is a diagnostic.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tsort.004
. "$PC_CORE/pc.sh"
pc_begin XCU.tsort.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a b\nb a\n' >cyc1 && printf 'a b\nb a\nc d\nd c\n' >cyc2 && printf 'a b\nb c\n' >acyc || pc_fail "cannot create input"
pc_test
tsort -w cyc1 >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "-w with one cycle gave status $rc, not 1"
tsort -w cyc2 >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 2 ] || pc_fail "-w with two separate cycles gave status $rc, not 2"
err=$(tsort -w cyc1 2>&1 >/dev/null </dev/null)
[ -n "$err" ] || pc_fail "-w with a cycle wrote no diagnostic"
got=$(tsort -w acyc 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}b${NL}c${NL}rc=0" ] || pc_fail "-w with no cycle did not give the order and status 0"
pc_pass
