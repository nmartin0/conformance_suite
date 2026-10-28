# return n ends the function with status n and the rest of the function is
# not run.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.return.001
. "$PC_CORE/pc.sh"
pc_begin XCU.return.001
pc_test
pcf() { return 3; echo notrun; }
r=$(pcf)
[ -z "$r" ] || pc_fail "commands after return ran: '$r'"
pcf
rc=$?
[ "$rc" -eq 3 ] || pc_fail "return 3 gave $rc"
pcf() { return 0; }
pcf
rc=$?
[ "$rc" -eq 0 ] || pc_fail "return 0 gave $rc"
for n in 1 127 128 255; do
	pcf() { return "$1"; }
	pcf "$n"
	rc=$?
	[ "$rc" -eq "$n" ] || pc_fail "return $n gave $rc"
done
pc_pass
